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
#include "PelicanTCPBlobServer.h"
#include "ThreadedBlobServer.h"
#include "utility/ConfigNode.h"

namespace pelican {

/**
 * @details
 * PelicanTCPBlobServer constructor
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(const ConfigNode& configNode )
      : AbstractOutputStream(configNode),
        _server(0),
        _connectionManager(0)
{
    // Initliase connection manager thread
    int port = configNode.getOption("connection", "port").toInt();

    // decide to run in threaded/ or non-threaded mode
    bool threaded = true;
    if( configNode.hasAttribute("threaded") )
    {
        threaded = ! QString::compare(configNode.getAttribute("threaded"), "false",  Qt::CaseInsensitive);
    }

    if( threaded )
        _server = new ThreadedBlobServer(port);
    else {
        _connectionManager = new TCPConnectionManager;
    }
}

/**
 * @details
 * PelicanTCPBlobServer destructor
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
    _server->quit();
    delete _server;
    delete _connectionManager;
}

/**
 * @details
 * method to tell if there are any clients listening for data
 */
int PelicanTCPBlobServer::clientsForStream(const QString& stream) const
{
    if( _server ) {
        return _server->clientsForStream(stream);
    }
    else {
        return _connectionManager->clientsForStream(stream);
    }
}

/**
 * @details
 * Send datablob to connected clients
 */
void PelicanTCPBlobServer::sendStream(const QString& streamName, const DataBlob* incoming)
{
    // Tell the threaded blob server to send data
    // This must be blocking, to avoid the DataBlob being recycled before
    // the data has been sent
    if( _server ) {
        _server->blockingSend(streamName, incoming);
    }
    if( _connectionManager ) {
        _connectionManager->send(streamName, incoming);
    }
}

/**
 */
void PelicanTCPBlobServer::stop()
{
    if( _server )
        _server->stop();
    else {
        _connectionManager->stop();
    }
}

void PelicanTCPBlobServer::listen()
{
    if( _server )
        _server->listen();
    else {
        _connectionManager->listen();
    }
}

/**
 * @details
 * Return the port bound to the server
 */
quint16 PelicanTCPBlobServer::serverPort() const
{
    if( _server )
        return _server->serverPort();
    else {
        return _connectionManager->serverPort();
    }
}

} // namespace pelican

