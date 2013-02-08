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

#include <iostream>

#include "output/TCPConnectionManager.h"
#include "comms/StreamDataRequest.h"
#include "comms/DataSupportResponse.h"
#include "comms/PelicanProtocol.h"
#include "comms/ServerRequest.h"
#include "utility/ConfigNode.h"
#include "comms/StreamData.h"
#include "data/DataBlob.h"

#include <QtCore/QBuffer>
#include <QtCore/QMutexLocker>
#include <QtNetwork/QTcpSocket>

namespace pelican {

/**
 * @details
 * TCPConnectionManager constructor
 */
TCPConnectionManager::TCPConnectionManager(quint16 port, QObject *parent)
                     : QObject(parent), _port(port),
                      _dataSupportStream("__streamInfo__")
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

    // Get new client connection
    QTcpSocket *client = _tcpServer->nextPendingConnection();
    if ( _processIncomming(client) ) {
        // Connect socket error() signals
        connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this,
                SLOT(connectionError(QAbstractSocket::SocketError)),
                Qt::DirectConnection);
        connect(client, SIGNAL(readyRead()), this,
                SLOT(_incomingFromClient()),
                Qt::DirectConnection);
    }
    else {
        client->close();
        delete client;
    }
}

void TCPConnectionManager::_incomingFromClient()
{
     _processIncomming(static_cast<QTcpSocket*>( sender() ) );
}

bool TCPConnectionManager::_processIncomming(QTcpSocket *client)
{

    // Wait for client to send in request type
    boost::shared_ptr<ServerRequest> request = _protocol->request(*client);

    switch ( request->type() )
    {
        case ServerRequest::DataSupport:
            {
                DataSupportResponse res( types() );
                _protocol->send(*client,res);
                // add the client to the stream update channel
                if( ! _clients[_dataSupportStream].contains(client) )
                {
                    //std::cout << "TCPConnectionManager: Adding new client for streamInfo\n";
                    _clients[_dataSupportStream].push_back(client);
                }
            }
            break;
        case ServerRequest::StreamData:
            {
                StreamDataRequest& req = static_cast<StreamDataRequest&>(*request);
                // Check for invalid data requirements
                if (req.isEmpty()) {
                    std::cerr << "Invalid client data requrements" << std::endl;
                    //client->close();
                    return false;
                }
                // Check data requirements
                DataSpecIterator it = req.begin();
                while(it != req.end()) {
                    // Add all client data requirement to type-client list
                    foreach(const QString& streamData, it->streamData() ) {
                        // Check if clients map already has the key, if so add client to list
                        //std::cout << "TCPConnectionManager: Adding new client for stream: "
                        //        << streamData.toStdString()  << std::endl;
                        if( ! _clients[streamData].contains(client) )
                            _clients[streamData].push_back(client);
                    }
                    ++it;
                }
            }
            break;
        default:
            {
                std::cerr << "TCPConnectionManager: Invalid client request" << std::endl;
                //client->close();
                return false;
            }
    }
    return true;

}

/**
 * @details
 * Send dataSupport Information to connected clients
 */
void TCPConnectionManager::_sendNewDataTypes()
{
    DataSupportResponse res( types() );

    clients_t clientListCopy;
    {
        // control access to the _clients
        QMutexLocker locker(&_mutex);
        clientListCopy = _clients[_dataSupportStream];
    }

    // iterate over all clients subscribed to the data support channel
    for(int i = 0; i < clientListCopy.size(); ++i ) {

        QTcpSocket* client =  clientListCopy[i];

        // Send data to client
        try {
            //std::cout << "Sending to:" << client->peerName().toStdString() << std::endl;
            Q_ASSERT( client->state() == QAbstractSocket::ConnectedState );
            _protocol->send(*client, res);
            client->flush();
            //std::cerr <<  "TCPConnectionManager: sending newdata types to client" << std::endl;
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
 * Send data to connected clients
 */
void TCPConnectionManager::send(const QString& streamName, const DataBlob* blob)
{

    QMutexLocker sendlocker(&_sendMutex);

    // Check if there are any client reading streamName type data
    if (_clients.contains(streamName) ) {
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
                //std::cout << "Sending blob of type " << blob->type().toStdString()
                //          << " on stream " << streamName.toStdString() << " to:"
                //          << client->peerName().toStdString() << std::endl;
                Q_ASSERT( client->state() == QAbstractSocket::ConnectedState );
                _protocol->send(*client, streamName, *blob);
                client->flush();
            }
            catch ( ... )
            {
                // kill the client if anything goes wrong
                std::cerr <<  "TCPConnectionManager: failed to send data to client" << std::endl;
                _killClient(client);
            }
        }
    }
    emit sent(blob); // let any blocked sends continue
                     // now the blob is sent.

    // Ensure we track the data streams and inform any interested
    // clients of updates.
    if( !_seenTypes.contains(streamName) )
    {
        _seenTypes.insert(streamName);
        _sendNewDataTypes();
    }
}

const QSet<QString>& TCPConnectionManager::types() const {
    return _seenTypes;
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

void TCPConnectionManager::stop()
{
    _tcpServer->close();
}

void TCPConnectionManager::listen()
{
    if (!_tcpServer -> listen( QHostAddress::Any, _port))
        std::cerr << QString("Unable to start QTcpServer: %1").arg( _tcpServer -> errorString()).toStdString();
}

/**
 * @details
 * Return the clients which are registered for a certain datablob stream
 */
int TCPConnectionManager::clientsForStream(const QString& type) const
{
    if( _clients.contains(type) )
        return _clients[type].size();
    return 0;
}

} // namespace pelican

