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

#ifndef THREADEDBLOBSERVER_H
#define THREADEDBLOBSERVER_H

/**
 * @file ThreadedBlobServer.h
 */

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <boost/shared_ptr.hpp>

namespace pelican {
class DataBlob;
class TCPConnectionManager;

/**
 * @ingroup c_output
 *
 * @class ThreadedBlobServer
 *
 * @brief
 *   Runs a TCPConnection manager in a seperate thread
 * @details
 *
 */

class ThreadedBlobServer : public QThread
{
    Q_OBJECT

    public:
        ThreadedBlobServer( quint16 port, QObject* parent=0 );
        ~ThreadedBlobServer();

        /// send in a seperate background
        void send(const QString& streamName, const DataBlob* incoming);

        /// send and block until sent
        void blockingSend(const QString& streamName, const DataBlob* incoming);

        /// return the port on which the server is listening
        qint16 serverPort() const;

        /// returns the number of clients listening for the specified stream
        int clientsForStream(const QString&) const;

        /// stop the server from accepting new connections
        void stop();

        /// restart the serveraafter a stop()
        void listen();

    protected:
        void run();

    signals:
        /// private signal to communicate internally
        void sending( const QString& , const DataBlob*);

    private slots:
        void sent(const DataBlob*);

    private:
        boost::shared_ptr<TCPConnectionManager> _manager;
        quint16 _port;
        QMap<const DataBlob*, QWaitCondition*> _waiting;
        QMutex _mutex;

};

} // namespace pelican
#endif // THREADEDBLOBSERVER_H
