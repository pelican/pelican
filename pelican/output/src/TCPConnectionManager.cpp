#include <QBuffer>
#include <QMutexLocker>
#include <iostream>
#include "TCPConnectionManager.h"

#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/comms/StreamData.h"
#include "pelican/utility/memCheck.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 *@details TCPConnectionManager 
 */
TCPConnectionManager::TCPConnectionManager(quint16 port, QObject *parent)
                     : QThread(parent), _port(port)
{
    _protocol = new PelicanProtocol; // TODO - make configurable
}

/**
 *@details
 */
TCPConnectionManager::~TCPConnectionManager()
{

    // Wait for main thread to finish.
    do { quit(); } while( ! wait(10) );
}


qint16 TCPConnectionManager::serverPort() const
{
    return _tcpServer->serverPort();
}

/**
 *@details
 */
void TCPConnectionManager::acceptClientConnection() 
{
    std::cout << "adding new client" << std::endl;
    // Get new client connection
    QTcpSocket *client = _tcpServer -> nextPendingConnection();
    Q_ASSERT( client->thread() == currentThread() );

    // Wait for client to send in request type
    boost::shared_ptr<ServerRequest> request = _protocol->request(*client);
    
    // Check if client sent the correct request
    if (static_cast<ServerRequest&>(*request).type() != ServerRequest::StreamData) {
        std::cerr << "Invalid client request" << std::endl;
        client->close();
        return;
    }

    StreamDataRequest& req = static_cast<StreamDataRequest&>(*request);

    // Check for invalid data requirements
    if (req.isEmpty()) {
        std::cerr << "Invalid client data requrements" << std::endl;
        client->close();
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

    // Connect socket disconnected() and error() signals
    //connect(client, SIGNAL(disconnected()), this, SLOT(connectionLost()), Qt::DirectConnection);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, 
                    SLOT(connectionError(QAbstractSocket::SocketError)), 
                    Qt::DirectConnection);

}

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
    for(int i=0; i < clientListCopy.size(); ++i ) {
        QTcpSocket* client =  clientListCopy[i];
        // Send data to client
        try {
            std::cout << "sending" << std::endl;
            _protocol->send(*client, streamName, blob);
        }
        catch ( ... ) 
        {
            // kill the client if anything goes wrong
            std::cerr <<  "TCPConnectionManager: failed to send data to client" << std::endl;
            _killClient(client); 
        }
    }
}

void TCPConnectionManager::_killClient(QTcpSocket* client)
{
    QMutexLocker locker(&_mutex);
    Q_ASSERT( client->thread() == currentThread() );
    foreach(const QString& stream, _clients.keys() ) {
        _clients[stream].removeAll(client);
    }
    client->disconnect();
    client->deleteLater();
}


/**
 *@details
 */
void TCPConnectionManager::connectionError(QAbstractSocket::SocketError)
{
    _killClient(static_cast<QTcpSocket*>(sender()));
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

    delete _tcpServer;
}

int TCPConnectionManager::clientsForType(const QString& type) const
{
    if( _clients.contains(type) )
        return _clients[type].size();
    return 0;
}

} // namespace pelican

