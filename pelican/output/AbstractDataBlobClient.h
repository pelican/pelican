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

#ifndef ABSTRACTDATABLOBCLIENT_H
#define ABSTRACTDATABLOBCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSet>
#include "data/DataSpec.h"
class QTcpSocket;

/**
 * @file AbstractDataBlobClient.h
 */

namespace pelican {
    class DataBlob;
    class Stream;
    class ServerRequest;
    class ServerResponse;
    class DataSupportResponse;
    class DataBlobResponse;
    class AbstractClientProtocol;

/**
 * @ingroup c_output
 *
 * @class AbstractDataBlobClient
 *
 * @brief
 *   Base class for all clients of the
 *   Pelican DataBlob server
 * @details
 *   Also provides an api for ommunications with
 *   the server
 */
class AbstractDataBlobClient : public QObject
{
    Q_OBJECT

    public:
        /// AbstractDataBlobClient constructor.
        AbstractDataBlobClient(QObject* parent=0);

        /// AbstractDataBlobClient destructor.
        virtual ~AbstractDataBlobClient();

        /// set the protocol to use
        //  (must be set before trying to connect)
        //  takes ownership of the protocol object
        void setProtocol( AbstractClientProtocol* p );

        /// set the host to listen to
        void setHost(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

        /// return the port of the host connected to
        quint16 port() const;

        /// returns the streams served by the blob server
        virtual QSet<QString> streams() { return _streams; };

        /// listen for the named streams
        virtual void subscribe( const QSet<QString>& streams );
        void subscribe( const QString& stream );
        QTcpSocket* socket() { return _tcpSocket; }

    protected:
        /// report verbose messages
        void verbose(const QString&, int level = 1);
        /// send a request to the server
        //  using the provided protocol
        bool sendRequest( const ServerRequest* req );

        // send server a request for StreamInfo
        bool requestStreamInfo();

        /// reimplement in your class
        //  for dealing with the various responses to
        //  the server
        virtual void serverError( ServerResponse* );
        virtual void dataSupport( DataSupportResponse* );
        virtual void dataReceived( DataBlobResponse* ) {};
        virtual void unknownResponse( ServerResponse* );

        virtual void onReconnect() {};
        virtual void onSubscribe( const QString& ) {};

    signals:
        void newData(const Stream& stream);
        void newStreamsAvailable();

    private: // methods
        bool _connect();

    private slots:
        void _response();
        void _reconnect();

    protected:
        QTcpSocket*  _tcpSocket;
        int _verbose;
        QString       _server;
        quint16       _port;


    private:
        AbstractClientProtocol* _protocol;
        bool _destructor;
        DataSpec _currentSubscription;
        QSet<QString> _subscriptions;
        QSet<QString> _streams;

};

} // namespace pelican

#endif // ABSTRACTDATABLOBCLIENT_H
