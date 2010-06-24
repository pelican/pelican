#include <QBuffer>
#include <iostream>
#include "PelicanTCPBlobServer.h"
#include <QTimer>

#include "pelican/utility/ConfigNode.h"
#include "ThreadedBlobServer.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details 
 * PelicanTCPBlobServer constructor
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(const ConfigNode& configNode, QObject* parent)
      : QObject(parent), AbstractBlobServer(configNode)
{
    // Initliase connection manager thread
    int port = configNode.getOption("connection", "port").toInt();
    _server = new ThreadedBlobServer(port,parent);
}

/**
 * @details 
 * PelicanTCPBlobServer destructor
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
    _server->quit();
    delete _server;
}

/**
 * @details
 * method to tell if there are any clients listening for data
 */
int PelicanTCPBlobServer::clientsForType(const QString& stream) const
{
    return _server->clientsForType(stream);
}

/**
 * @details
 * Send datablob to connected clients 
 */
void PelicanTCPBlobServer::send(const QString& streamName, const DataBlob* incoming)
{
    // Tell the threaded blob server to send data
    _server->send(streamName, incoming);
}

/**
 * @details
 * Return the port bound to the server
 */
quint16 PelicanTCPBlobServer::serverPort() const
{
    return _server->serverPort();
}

} // namespace pelican

