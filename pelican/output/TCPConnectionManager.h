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

#ifndef TCPCONNECTIONMANAGER_H
#define TCPCONNECTIONMANAGER_H

/**
 * @file TCPConnectionManager.h
 */

#include <QtNetwork/QTcpServer>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QMap>

#include "utility/ConfigNode.h"

namespace pelican {

class AbstractProtocol;
class DataBlob;

/**
 * @ingroup c_output
 *
 * @class TCPConnectionManager
 *
 * @brief
 *   TCP Connection Management thread
 * @details
 *
 */

class TCPConnectionManager : public QObject
{
    Q_OBJECT

    public:
        TCPConnectionManager(quint16 port=0, QObject *parent = 0);
        ~TCPConnectionManager();
        qint16 serverPort() const;
        int clientsForStream(const QString&) const;
        // stop listening for new connections, but preserve state
        void stop();
        // start listening for new connections, after a stop()
        void listen();

    protected:
        virtual void run();
        void _killClient(QTcpSocket*);
        const QSet<QString>& types() const;
        void _sendNewDataTypes();
        bool _processIncomming(QTcpSocket*);

    public slots:
        void send(const QString& streamName, const DataBlob* incoming);

    private:
        typedef QList<QTcpSocket*>         clients_t;
        quint16                            _port;
        QMap<QString, clients_t >          _clients;
        QTcpServer*                        _tcpServer;
        QMutex                             _mutex; // controls access to _clients
        QMutex                             _sendMutex; // controls access to send method
        AbstractProtocol*                  _protocol;
        QSet<QString>                      _seenTypes;          // record what types have been seen (via send() )
        const QString                      _dataSupportStream;  // the name of the subscrition stream for data support requests

    private slots:
        void connectionError(QAbstractSocket::SocketError socketError);
        void acceptClientConnection();
        void _incomingFromClient();

    signals:
        void sent(const DataBlob*);

};

} // namespace pelican
#endif // TCPCONNECTIONMANAGER_H
