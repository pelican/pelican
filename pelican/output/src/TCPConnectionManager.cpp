#include <QBuffer>
#include <iostream>
#include "TCPConnectionManager.h"

#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/comms/StreamData.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 *@details TCPConnectionManager 
 */
TCPConnectionManager::TCPConnectionManager(const   ConfigNode& configNode,  QObject *parent)
                     : QThread(parent), _port(0)
{
    // Set TCP Server connection options
    int port = configNode.getOption("connection", "port").toInt();
    _port = port > 0 ? port : 9000;
}

/**
 *@details
 */
TCPConnectionManager::~TCPConnectionManager()
{
    // TODO: Disconnect all clients

    // Clear up TCP server
    delete _tcpServer;

    // Wait for main thread to finish.
    if (isRunning()) while (!isFinished()) quit();
    wait();
}

/**
 *@details
 */
void TCPConnectionManager::acceptClientConnection() 
{
    // Get new client connection
    QTcpSocket *client = _tcpServer -> nextPendingConnection();

    // Wait for client to send in request type
    PelicanProtocol proto;
    boost::shared_ptr<ServerRequest> request = proto.request(*client);
    
    // Check if client sent the correct request
    if (static_cast<ServerRequest&>(*request).type() != ServerRequest::StreamData) {
        std::cerr << "Invalid client request" << std::endl;
        client -> close();
        return;
    }    

    StreamDataRequest& req = static_cast<StreamDataRequest&>(*request);

    // Check for invalid data requirements
    if (req.isEmpty()) {
        std::cerr << "Invalid client data requrements" << std::endl;
        client -> close();
        return;
    }

    // Check data requirements
    DataRequirementsIterator it = req.begin();
    while(it != req.end()) {

        // Add all client data requirement to type-client list
        foreach(QString streamData, it -> streamData() ) {

            // Check if clients map already has the key, if so add client to list
            if ( _clients.contains(streamData) )
                _clients[streamData].push_back(client);

            // Create new map entry with empty clients list, and add client
            else {
                QList<QTcpSocket *> socketList;
                socketList.push_back(client);
                _clients.insert(streamData, socketList);
            }
        }
        
        ++it;
    }

    // Connect socket disconnected() and error() signals
    connect(client, SIGNAL(disconnected()), this, SLOT(connectionLost()), Qt::DirectConnection);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, 
                    SLOT(connectionError(QAbstractSocket::SocketError)), 
                    Qt::DirectConnection);

    std::cout << "Added client" << std::endl;
}

void TCPConnectionManager::send(const QString& streamName, const QByteArray& incoming)
{
    // Check if there are any client reading streamName type data
    if (!_clients.contains(streamName) )
        return;  // No client to stream to

    // Create data object to stream
    PelicanProtocol proto;
    StreamData streamData(streamName, (void *) incoming.data(), incoming.size());
    streamData.setId("id");
    AbstractProtocol::StreamData_t data;
    data.append(&streamData);

    foreach(QTcpSocket* client, _clients[streamName] ) {
        // Send data to client
        proto.send(*client, data);
        // Wait for data to be sent, with x timeout
        // & check return value for bytes written
        // kill the client if anything goes wrong
        
    }
}

/**
 *@details
 */
void TCPConnectionManager::connectionLost() 
{
    std::cout << "Client connection lost" << std::endl;
}

/**
 *@details
 */
void TCPConnectionManager::connectionError(QAbstractSocket::SocketError socketError) 
{
    std::cout << "Client error occured: " << socketError << std::endl;
}

/**
 *@details
 */
void TCPConnectionManager::run() 
{
    // start TCP Server, which listens for incoming connections
    _tcpServer = new QTcpServer;

    if (!_tcpServer -> listen( QHostAddress::Any, _port))
        std::cerr << QString("Unable to start QTcpServer: %1").arg( _tcpServer -> errorString()).toStdString();
    else {
        connect(_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptClientConnection()), Qt::DirectConnection );
        exec();
    }
}

} // namespace pelican

