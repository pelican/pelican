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

#ifndef CLIENTTESTSERVER_H
#define CLIENTTESTSERVER_H

/**
 * @file ClientTestServer.h
  */

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtNetwork/QTcpServer>


namespace pelican {

/**
 * @ingroup c_utility
 *
 * @class ClientTestServer
 *
 * @brief
 *    A server for unit testing client appliants.
 *    The test server will connect to an available port and record
 *    any data sent to it
 * @details
 *
 */
class ClientTestServer : public QThread
{
    Q_OBJECT

    public:
        /// ClientTestServer constructor.
        ClientTestServer( QObject* parent = 0 );

        /// ClientTestServer destructor.
        ~ClientTestServer();

        /// returns the hostname for the server
        const QString& hostname() const;

        /// return the port number the server is listening on
        //  A free port is assigned automatically.
        quint16 port() const;

        /// returns true if the server is listening for incomming
        //  requests
        bool isListening() const;

        /// returns a dump of all data received since the last call
        //  to this function
        QByteArray dataReceived();

    protected:
        void run();

    protected slots:
        void acceptClientConnection();
        void _incomingFromClient();
        void _connectionError(QAbstractSocket::SocketError);

    private:
        QByteArray _received;
        QString _hostname;
        QTcpServer* _server;
};

} // namespace pelican

#endif // CLIENTTESTSERVER_H
