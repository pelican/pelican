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

#ifndef PELICANPORTSERVER_H
#define PELICANPORTSERVER_H


#include <QtNetwork/QTcpServer>

/**
 * @file PelicanPortServer.h
 */

namespace pelican {
class AbstractProtocol;
class DataManager;

/**
 * @ingroup c_server
 *
 * @class PelicanPortServer
 *
 * @brief
 *    Class that listens on a specific port
 * @details
 *    Internal class used by PelicanServer
 *    Each port is associated with a single protocol
 *
 */
class Session;

class PelicanPortServer : public QTcpServer
{

    Q_OBJECT

    public:
        PelicanPortServer(AbstractProtocol* proto, DataManager* data, QObject* parent=0 );
        ~PelicanPortServer();

        void setVerbosity(int level) { _verboseLevel=level; };

    protected:
        /// Reimplemented from QTcpServer.
        void incomingConnection(int socketDescriptor);

    private:
        AbstractProtocol* _proto;
        DataManager* _data;
        int _verboseLevel;
};

} // namespace pelican
#endif // PELICANPORTSERVER_H
