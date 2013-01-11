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

#ifndef SESSION_H
#define SESSION_H

#include <QtCore/QThread>
#include <QtCore/QList>
#include <QtNetwork/QTcpSocket>
#include <string>

/**
 * @file Session.h
 */

namespace pelican {

class ServerRequest;
class ServiceDataRequest;
class StreamDataRequest;
class LockedData;
class AbstractProtocol;
class DataManager;

/**
 * @ingroup c_server
 *
 * @class Session
 *
 * @brief
 * Class to process a single server request.
 *
 * @details
 *
 */
class Session : public QThread
{
    Q_OBJECT

    public:
        /// Constructs a server session.
        Session(int socketDescriptor, AbstractProtocol* proto,
                DataManager* data,
                QObject* parent = 0);

        /// Destroys the server session.
        ~Session();

    public:
        /// Runs the session thread processing a request.
        /// (implements run method of QThread).
        void run();

        /// Process a request to the server sending the appropriate response.
        void processRequest(const ServerRequest&, QIODevice&, unsigned timeout = 0 );

        // set the verbosity level ( 0 = off )
        void setVerbosity(int level);

    protected:
        /// Returns the first valid stream data with associated service data.
        QList<LockedData> processStreamDataRequest(const StreamDataRequest& req,
                unsigned timeout = 0);

        QList<LockedData> processServiceDataRequest(const ServiceDataRequest& req);
        void verbose( const QString& msg, int verboseLevel = 1 );

    signals:
        void error(QTcpSocket::SocketError socketError);

    private:
        int _socketDescriptor;
        DataManager* _dataManager;
        AbstractProtocol* _protocol;
        int _verboseLevel;
        std::string _clientInfo;
        friend class SessionTest; // unit test
};

} // namespace pelican

#endif // SESSION_H
