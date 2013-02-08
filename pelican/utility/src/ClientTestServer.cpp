/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
