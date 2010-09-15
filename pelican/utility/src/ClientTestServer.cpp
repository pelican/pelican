#include "ClientTestServer.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <iostream>

namespace pelican {

/**
 * @details Constructs a ClientTestServer object.
 */
ClientTestServer::ClientTestServer( QObject* parent )
    : QThread( parent ), _hostname("localhost"), _server(0)
{
    start();
    do{ msleep(2); } while( ! _server );
}

/**
 * @details Destroys the ClientTestServer object.
 */
ClientTestServer::~ClientTestServer()
{
    do { quit(); } while(!wait(10));
}

const QString& ClientTestServer::hostname() const
{
    return _hostname;
}

quint16 ClientTestServer::port() const
{
    Q_ASSERT( _server );
    return _server->serverPort();
}

/// returns true if the server is listening for incomming
//  requests
bool ClientTestServer::isListening() const
{
    Q_ASSERT( _server );
    return _server->isListening();
}

void ClientTestServer::run()
{
    _server = new QTcpServer;
    if ( ! _server->listen(QHostAddress::Any, 0) ) {
        std::cerr << QString("Unable to start QTcpServer: %1").arg( _server -> errorString()).toStdString();
        return;
    }
    else
        connect(_server, SIGNAL(newConnection()), this, SLOT(acceptClientConnection()), Qt::DirectConnection );
    exec();
    delete _server;
}

QByteArray ClientTestServer::dataReceived()
{
    QByteArray temp = _received;
    _received.clear();
    return temp;
}

void ClientTestServer::acceptClientConnection()
{
    QTcpSocket *client = _server->nextPendingConnection();
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(_connectionError(QAbstractSocket::SocketError)),
            Qt::DirectConnection);
    connect( client, SIGNAL(readyRead()), this,
            SLOT(_incomingFromClient()), Qt::DirectConnection);
}

void ClientTestServer::_incomingFromClient()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>( sender() );
    // record the data sent
    while( socket->bytesAvailable() > 0 )
    {
        _received += socket->readAll();
    }
}

void ClientTestServer::_connectionError(QAbstractSocket::SocketError)
{
    QTcpSocket* socket = static_cast<QTcpSocket*>( sender() );
    delete socket;
}

} // namespace pelican
