#include <QBuffer>
#include <iostream>
#include "PelicanTCPBlobServer.h"
 #include <QTimer>

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details PelicanTCPBlobServer 
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(const ConfigNode& configNode, QObject* parent)
      : QObject(parent)
{
    // Initliase connection manager thread
    _connectionManager = new TCPConnectionManager(configNode, parent);
    _clients = _connectionManager -> getClientsReference();
    _connectionManager -> start();
}

/**
 *@details
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
    //TODO: Destruct connection Manager Thread
}

/**
 *@details
  */
void PelicanTCPBlobServer::send(const QString& streamName, const QByteArray& incoming)
{
    // Tell connection manager to send data, somehow...
}

} // namespace pelican

