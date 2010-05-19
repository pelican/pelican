#include <QBuffer>
#include <iostream>
#include "PelicanTCPBlobServer.h"
#include <QTimer>

#include "pelican/utility/ConfigNode.h"
#include "ThreadedBlobServer.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details PelicanTCPBlobServer 
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(const ConfigNode& configNode, QObject* parent)
      : QObject(parent)
{
    // Initliase connection manager thread
    int port = configNode.getOption("connection", "port").toInt();
//    _connectionManager = new TCPConnectionManager(port, parent);
//    _connectionManager->start();
    _server = new ThreadedBlobServer(port,parent);
}

/**
 *@details
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
    _server->quit();
    delete _server;
    //_connectionManager->quit();
    //delete _connectionManager;
}

/**
 *@details
  */
void PelicanTCPBlobServer::send(const QString& streamName, const DataBlob& incoming)
{
    // Tell connection manager to send data, somehow...
    //_connectionManager->send(streamName,incoming);
    _server->send(streamName,incoming);
}

quint16 PelicanTCPBlobServer::serverPort() const
{
    //return _connectionManager->serverPort();
    return _server->serverPort();
}

} // namespace pelican

