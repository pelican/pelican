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

void ThreadedBlobServer::run()
{
    // Create a connection manager in the thread and run it inside the event loop
    // using a boost shared_ptr will ensure it gets deleted when we leave the scope
    _manager.reset( new TCPConnectionManager(_port) );
    bool res = connect( this, SIGNAL( sending(const QString&, const DataBlob*) ),
             _manager.get(), SLOT( send( const QString&, const DataBlob* )));
    Q_ASSERT( res );
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

} // namespace pelican
