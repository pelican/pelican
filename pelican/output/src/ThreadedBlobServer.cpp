#include "pelican/output/ThreadedBlobServer.h"
#include "pelican/output/TCPConnectionManager.h"

#include <QtCore/QTimer>

#include "pelican/utility/memCheck.h"

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

void ThreadedBlobServer::run()
{
    // Create a connection manager in the thread and run it inside the event loop
    // using a boost shared_ptr will ensure it gets deleted when we leave the scope
    _manager.reset( new TCPConnectionManager(_port) );
    exec();
}

/*
 * @details
 * emit a signal to the connection manager in the thread
 */
void ThreadedBlobServer::send(const QString& streamName, const DataBlob& blob)
{
    _sendStream = streamName;
    _sendBlob = &blob;
    QTimer::singleShot( 0, this , SLOT( _send()) );
}

/*
 * @details
 * This method should be called as a slot - ensures we call it in the thread
 * rather than the callers thread
 */
void ThreadedBlobServer::_send()
{
    _manager->send( _sendStream, *_sendBlob );
}

} // namespace pelican
