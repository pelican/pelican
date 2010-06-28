#include "pelican/output/ThreadedBlobServer.h"
#include "pelican/output/TCPConnectionManager.h"

#include <QtCore/QTimer>

#include "pelican/utility/memCheck.h"

#include <iostream>

namespace pelican {


/**
 *@details ThreadedBlobServer 
 */
ThreadedBlobServer::ThreadedBlobServer( quint16 port, QObject* parent )
    : QThread( parent ), _port(port)
{
    start();
}

/**
 *@details
 * ensures the thread is stopped before we delete the object
 */
ThreadedBlobServer::~ThreadedBlobServer()
{
    do { quit(); } while( ! wait(10) ); }

qint16 ThreadedBlobServer::serverPort() const
{
    return _manager->serverPort();
}

/**
 * @details
 * method to tell if there are any clients listening for data
 */
int ThreadedBlobServer::clientsForType(const QString& stream) const
{
    return _manager->clientsForType(stream);
}

void ThreadedBlobServer::run()
{
    // Create a connection manager in the thread and run it inside the event loop
    // using a boost shared_ptr will ensure it gets deleted when we leave the scope
    _manager.reset( new TCPConnectionManager(_port) );
    bool res = connect( this, SIGNAL( sending(const QString&, const DataBlob*) ),
             _manager.get(), SLOT( send( const QString&, const DataBlob* )));
    Q_ASSERT( res );
    res = connect( _manager.get(), SIGNAL( sent(const DataBlob*) ),
            this , SLOT( sent( const DataBlob* )), Qt::DirectConnection);
    exec();
}

/*
 * @details
 * emit a signal to the connection manager in the thread
 */
void ThreadedBlobServer::send(const QString& streamName, const DataBlob* blob)
{
    emit sending(streamName, blob);
}

/**
 * @details
 * Send datablob to connected clients, and block until data is sent
 */
void ThreadedBlobServer::blockingSend(const QString& streamName, const DataBlob* incoming)
{
    _mutex.lock();
    _waiting[incoming] = new QWaitCondition; // ensure waiting object is created before we send
    send(streamName, incoming ); // Tell the threaded blob server to send data
    _waiting[incoming]->wait(&_mutex); // go to sleep until send() has completed
    delete _waiting.take(incoming);
    _mutex.unlock();
}

/**
 * @details
 * This slot is called by the TCP Connection manager whenever it has finished sending data
 * It should be run in the TCPManager's thread, and is used to wake up any thread that has called 
 * blockingSend()
 */
void ThreadedBlobServer::sent(const DataBlob* blob)
{
    Q_ASSERT( currentThread() == this );
    if( _waiting.contains(blob) )
    {
        QMutexLocker locker(&_mutex);
        _waiting[blob]->wakeAll();
    }
}

} // namespace pelican
