#include <iostream>
#include "PelicanTCPBlobServer.h"
#include "ThreadedBlobServer.h"
#include "pelican/utility/ConfigNode.h"

namespace pelican {

/**
 * @details
 * PelicanTCPBlobServer constructor
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(const ConfigNode& configNode )
      : AbstractOutputStream(configNode),
        _server(0),
        _connectionManager(0)
{
    // Initliase connection manager thread
    int port = configNode.getOption("connection", "port").toInt();

    // decide to run in threaded/ or non-threaded mode
    bool threaded = true;
    if( configNode.hasAttribute("threaded") )
    {
        threaded = ! QString::compare(configNode.getAttribute("threaded"), "false",  Qt::CaseInsensitive);
    }

    if( threaded )
        _server = new ThreadedBlobServer(port);
    else {
        _connectionManager = new TCPConnectionManager;
    }
}

/**
 * @details
 * PelicanTCPBlobServer destructor
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
    _server->quit();
    delete _server;
    delete _connectionManager;
}

/**
 * @details
 * method to tell if there are any clients listening for data
 */
int PelicanTCPBlobServer::clientsForStream(const QString& stream) const
{
    if( _server ) {
        return _server->clientsForStream(stream);
    }
    else {
        return _connectionManager->clientsForStream(stream);
    }
}

/**
 * @details
 * Send datablob to connected clients
 */
void PelicanTCPBlobServer::sendStream(const QString& streamName, const DataBlob* incoming)
{
    // Tell the threaded blob server to send data
    // This must be blocking, to avoid the DataBlob being recycled before
    // the data has been sent
    if( _server ) {
        _server->blockingSend(streamName, incoming);
    }
    if( _connectionManager ) {
        _connectionManager->send(streamName, incoming);
    }
}

/**
 */
void PelicanTCPBlobServer::stop()
{
    if( _server )
        _server->stop();
    else {
        _connectionManager->stop();
    }
}

void PelicanTCPBlobServer::listen()
{
    if( _server )
        _server->listen();
    else {
        _connectionManager->listen();
    }
}

/**
 * @details
 * Return the port bound to the server
 */
quint16 PelicanTCPBlobServer::serverPort() const
{
    if( _server )
        return _server->serverPort();
    else {
        return _connectionManager->serverPort();
    }
}

} // namespace pelican

