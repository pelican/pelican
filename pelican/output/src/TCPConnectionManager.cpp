#include <iostream>

#include "pelican/output/TCPConnectionManager.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/comms/StreamData.h"

#include <QtCore/QBuffer>
#include <QtCore/QMutexLocker>
#include <QtNetwork/QTcpSocket>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details 
 * TCPConnectionManager constructor
 */
TCPConnectionManager::TCPConnectionManager(quint16 port, QObject *parent)
                     : QObject(parent), _port(port)
{
    _protocol = new PelicanProtocol; // TODO - make configurable
    _tcpServer = new QTcpServer;
    run();
}

/**
 * @details
 * TCPConnectionManager destructor
 */
TCPConnectionManager::~TCPConnectionManager()
{
    delete _tcpServer;
}


/**
 * @details
 * Return the port bound to the server
 */
qint16 TCPConnectionManager::serverPort() const
{
    return _tcpServer->serverPort();
}

/**
 * @details
 * Accpet client connections for data
 */
void TCPConnectionManager::acceptClientConnection() 
{
    std::cout << "Adding new client" << std::endl;
    
    // Get new client connection
    QTcpSocket *client = _tcpServer -> nextPendingConnection();

    // Wait for client to send in request type
    boost::shared_ptr<ServerRequest> request = _protocol->request(*client);
    
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
        foreach(const QString& streamData, it -> streamData() ) {
            // Check if clients map already has the key, if so add client to list
            _clients[streamData].push_back(client);
        }

        ++it;
    }

    // Connect socket error() signals
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, 
                    SLOT(connectionError(QAbstractSocket::SocketError)), 
                    Qt::DirectConnection);
}

/**
 * @details
 * Send data to connected clients
 */
void TCPConnectionManager::send(const QString& streamName, const DataBlob& blob)
{
    QMutexLocker sendlocker(&_sendMutex);

    // Check if there are any client reading streamName type data
    if (!_clients.contains(streamName) )
        return;  // No client to stream to

    clients_t clientListCopy;
    {
        // control access to the _clients
        QMutexLocker locker(&_mutex);
        clientListCopy = _clients[streamName];
    }

    for(int i = 0; i < clientListCopy.size(); ++i ) {

        QTcpSocket* client =  clientListCopy[i];

        // Send data to client
        try {
            std::cout << "Sending to:" << client->peerName().toStdString() << std::endl;
            Q_ASSERT( client -> state() == QAbstractSocket::ConnectedState );
            _protocol -> send(*client, streamName, blob);
            client -> flush();
            std::cout << "Finished sending" << std::endl;
        }
        catch ( ... ) 
        {
            // kill the client if anything goes wrong
            std::cerr <<  "TCPConnectionManager: failed to send data to client" << std::endl;
            _killClient(client); 
        }
    }
}

/**
 * @details
 * Disconnect client socket
 */
void TCPConnectionManager::_killClient(QTcpSocket* client)
{
    QMutexLocker locker(&_mutex);
    foreach(const QString& stream, _clients.keys() ) {
        _clients[stream].removeAll(client);
    }
    client->disconnect();
    client->deleteLater();
}


/**
 * @details
 * Socket error slot
 */
void TCPConnectionManager::connectionError(QAbstractSocket::SocketError)
{
    _killClient(static_cast<QTcpSocket*>(sender()));
}

/**
 * @details
 * Run the TCP server, listening for incoming client requests
 */
void TCPConnectionManager::run() 
{
    // start TCP Server, which listens for incoming connections
    if (!_tcpServer -> listen( QHostAddress::Any, _port))
        std::cerr << QString("Unable to start QTcpServer: %1").arg( _tcpServer -> errorString()).toStdString();
    else
        connect(_tcpServer, SIGNAL(newConnection()), this, SLOT(acceptClientConnection()), Qt::DirectConnection );
}

/**
 * @details
 * Return the clients which are registered for a certain datablob type
 */
int TCPConnectionManager::clientsForType(const QString& type) const
{
    if( _clients.contains(type) )
        return _clients[type].size();
    return 0;
}

} // namespace pelican

