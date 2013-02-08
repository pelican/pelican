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

#ifndef PELICANSERVER_H
#define PELICANSERVER_H

/**
 * @file PelicanServer.h
 */

#include <QtNetwork/QTcpServer>
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QPair>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>

namespace pelican {

class AbstractChunker;
class AbstractProtocol;
class ChunkerManager;
class Config;
class PelicanPortServer;

/**
 * @ingroup c_server
 *
 * @class PelicanServer
 *
 * @brief
 * The main data server class to spawn out Sessions
 *
 * @details
 * Initialises the server according to configuration options etc
 * and listens on the specified socket. On an connection a Session
 * object is spawned in another thread
 *
 * \par Example of using the server:
 * \include examples/mainServerExample.cpp
 */

class PelicanServer : public QThread
{
    Q_OBJECT

    public:
        PelicanServer(const Config* config, QObject* parent=0);
        ~PelicanServer();

        /// Associate an incoming port with a particular protocol.
        /// Ownership of AbstractProtocol is transferred to this class.
        void addProtocol(AbstractProtocol*, quint16 port);

        /// Adds a stream chunker.
        void addStreamChunker(QString type, QString name = QString());

        /// Adds a service chunker.
        void addServiceChunker(QString type, QString name = QString());

        /// Returns true if the server is ready.
        bool isReady() {QMutexLocker locker(&_mutex); return _ready;}

        /// set the verbosity of the server
        void setVerbosity(int level);

    protected:
        /// Code to be executed in the thread, via start().
        void run();
        void verbose(const QString& msg, int level=1);

    private:
        QMap<quint16,AbstractProtocol*> _protocolPortMap;
        QMutex _mutex;
        ChunkerManager* _chunkerManager;
        bool _ready;
        const Config* _config;
        int _verboseLevel;
};

} // namespace pelican

#endif // PELICANSERVER_H
