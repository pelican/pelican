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

#include "AbstractDataBlobClient.h"
#include <QtNetwork/QTcpSocket>
#include <QtCore/QSet>
#include <iostream>

#include "comms/AbstractClientProtocol.h"
#include "comms/ServerResponse.h"
#include "comms/StreamDataRequest.h"
#include "comms/DataSupportRequest.h"
#include "comms/DataSupportResponse.h"
#include "comms/DataBlobResponse.h"

namespace pelican {


/**
 * @details Constructs a AbstractDataBlobClient object.
 */
AbstractDataBlobClient::AbstractDataBlobClient(QObject* parent)
    : QObject(parent), _verbose(0), _protocol(0), _destructor(false)
{
    _tcpSocket = new QTcpSocket;
    connect(_tcpSocket, SIGNAL( readyRead()),
                         this, SLOT( _response() ));
    connect(_tcpSocket, SIGNAL( disconnected()),
                         this, SLOT( _reconnect() ));
}

/**
 * @details Destroys the AbstractDataBlobClient object.
 */
AbstractDataBlobClient::~AbstractDataBlobClient()
{
    _destructor = true;
    delete _tcpSocket;
    delete _protocol;
}

void AbstractDataBlobClient::setProtocol( AbstractClientProtocol* p ){
   if( _protocol ) { delete _protocol; }
   _protocol = p;
}

void AbstractDataBlobClient::setPort(quint16 port)
{
    _port = port;
}

quint16 AbstractDataBlobClient::port() const
{
    return _port;
}


void AbstractDataBlobClient::setHost(const QString& ipaddress)
{
    _server = ipaddress;
}

void AbstractDataBlobClient::verbose(const QString& msg, int level)
{
    if( level <= _verbose )
    {
        std::cout << "DataBlobClient: " << msg.toStdString() << std::endl;
    }
}

void AbstractDataBlobClient::subscribe(const QSet<QString>& streams)
{

    // Define the data type which the client will accept and send request
    if( streams.size() > 0 ) {
        StreamDataRequest req;
        DataSpec require;
        foreach( const QString& stream, streams )
        {
            verbose(QString("Subscribing to stream : \"") + stream + " \"" );
            onSubscribe( stream );
            require.addStreamData(stream);
        }
        if( require != _currentSubscription )
        {
            req.addDataOption(require);
            sendRequest(&req);
            _subscriptions.unite(streams);
        }
    }

}

void AbstractDataBlobClient::subscribe(const QString& stream)
{
    QSet<QString> set;
    set.insert(stream);
    subscribe(set);
}

bool AbstractDataBlobClient::sendRequest( const ServerRequest* req )
{
    // construct request and connect to the port
    QByteArray data = _protocol->serialise(*req);

    bool sent = _connect() ;
    if ( sent )
    {
        _tcpSocket->write(data);
        _tcpSocket->waitForBytesWritten(data.size());
        _tcpSocket->flush();
    }
    return sent;
}

bool AbstractDataBlobClient::requestStreamInfo()
{
    DataSupportRequest req;
    return sendRequest(&req);
}

// default implementation
void AbstractDataBlobClient::serverError( ServerResponse* r ) {
    std::cerr << "AbstractDataBlobClient: Server Error: "
              << r->message().toStdString()
              << std::endl;
}

void AbstractDataBlobClient::dataSupport( DataSupportResponse* res ) {
#ifdef BROKEN_QT_SET_HEADER
    _streams = res->streamData();
    _streams.unite(res->serviceData());
#else
    _streams = res->streamData() + res->serviceData();
#endif
}

void AbstractDataBlobClient::unknownResponse( ServerResponse* )
{
    std::cerr << "DataBlobClient: Unknown Response" << std::endl;
}

// handle the server response
void AbstractDataBlobClient::_response()
{
    boost::shared_ptr<ServerResponse> r = _protocol->receive(*_tcpSocket);
    // Check what type of response we have
    // and call the appropriate handling method
    switch( r -> type() ) {
        case ServerResponse::Error:  // Error occurred!!
            verbose("ServerResponse: error");
            serverError( r.get() );
            break;
        case ServerResponse::DataSupport:
            verbose("ServerResponse: DataSupport information");
            dataSupport( static_cast<DataSupportResponse*>(r.get()) );
            break;
        case ServerResponse::Blob:
            verbose("ServerResponse: DataBlob received");
            dataReceived( static_cast<DataBlobResponse*>(r.get()) );
            break;
        default:
            verbose("ServerResponse: unknown response");
            unknownResponse( r.get() );
            break;
     }
}

bool AbstractDataBlobClient::_connect()
{
    while (_tcpSocket->state() == QAbstractSocket::UnconnectedState) {
       _tcpSocket->connectToHost( _server, _port );
       if (!_tcpSocket->waitForConnected(5000)
           || _tcpSocket->state() == QAbstractSocket::UnconnectedState) {
           std::cerr << "Client could not connect to server:" << _server.toStdString() << " port:" << _port << std::endl;
           sleep(2);
           continue;
       }
    }
    return true;
}

void AbstractDataBlobClient::_reconnect()
{
    if( ! _destructor ) {
        verbose( "DataBlobClient: Connection lost - reconnecting()", 1);
        _currentSubscription.clear();
        subscribe( _subscriptions );
        onReconnect();
    }
}
} // namespace pelican
