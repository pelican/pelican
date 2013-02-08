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

#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H

#include "output/TCPConnectionManager.h"
#include "output/AbstractOutputStream.h"

/**
 * @file PelicanTCPBlobServer.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;
class ThreadedBlobServer;

/**
 * @ingroup c_output
 *
 * @class PelicanTCPBlobServer
 *
 * @brief
 *   Server that sends the same data from the queue to all connected peers
 * @details
 *   Specify the port to listen to with a connection tag
 *   e.g.
 *   @code
 *   <connection port="1234">
 *   @endcode
 *
 *   The server has two modes : threaded (default) or non-threaded
 *
 *   In Threaded node, the clients will be served by a seperate thread. This will give connecting
 *   clients a reasonable response time whatever other components of the system are doing.
 *
 *   Non-threaded mode allows you to eliminate the performance cost of running a seperate thread
 *   but the server can only respond to clients requests during the calling threads event loop.
 *   This is acceptable when this called very frequently e.g. in a very fast running pipeline.
 *   WARNING - this mode is not yet tested TODO
 *   set the threaded="false" attribute in the configuration file to select this mode
 */

class PelicanTCPBlobServer : public AbstractOutputStream
{
    public:
        PelicanTCPBlobServer( const ConfigNode& config );
        ~PelicanTCPBlobServer();
        quint16 serverPort() const;
        // stop the server from accepting connections
        void stop();
        // start listening for incomming connections
        void listen();

        /// return the number of clients listening to a specified stream
        int clientsForStream(const QString& stream) const;

    protected:
        virtual void sendStream(const QString& streamName, const DataBlob* dataBlob);

    private:
        ThreadedBlobServer* _server;
        TCPConnectionManager*  _connectionManager;

    private:
        friend class PelicanTCPBlobServerTest;

};

PELICAN_DECLARE(AbstractOutputStream, PelicanTCPBlobServer )

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H
