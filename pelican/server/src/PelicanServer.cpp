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

#include "server/PelicanServer.h"

#include "comms/AbstractProtocol.h"
#include "server/AbstractChunker.h"
#include "server/ChunkerManager.h"
#include "server/DataManager.h"
#include "server/DataReceiver.h"
#include "comms/PelicanProtocol.h"
#include "server/PelicanPortServer.h"
#include "utility/Config.h"

#include <boost/shared_ptr.hpp>

#include <QtCore/QtGlobal>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDebug>

#include <iostream>

namespace pelican {

/**
 * @details
 * Creates a new Pelican server, which in turn creates a chunker manager.
 */
PelicanServer::PelicanServer(const Config* config, QObject* parent) :
    QThread(parent), _verboseLevel(0)
{
    _config = config;
    _ready = false;

    // Create the chunker manager.
    _chunkerManager = new ChunkerManager(config);
}

/**
 * @details
 * Destroys the Pelican server.
 */
PelicanServer::~PelicanServer()
{
    // Delete the chunker manager (must be destroyed before data manager).
    delete _chunkerManager;

    // Wait for the thread to finish.
    do quit(); while (!wait(10));

    // Delete the protocols.
    foreach (AbstractProtocol* protocol, _protocolPortMap)
        delete protocol;
}

/**
 * @details
 * Adds the given \p protocol to the given \p port.
 * The class takes ownership of \p protocol.
 *
 * @param proto A pointer to the allocated protocol.
 * @param port  The incoming port number to use.
 */
void PelicanServer::addProtocol(AbstractProtocol* protocol, quint16 port)
{
    if ( _protocolPortMap.contains(port) ) {
        delete protocol;
        throw QString("Cannot map multiple protocols to port %1").arg(port);
    }
    _protocolPortMap[port] = protocol;
}

/**
 * @details
 * Adds a stream chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
void PelicanServer::addStreamChunker(QString type, QString name)
{
    _chunkerManager->addStreamChunker(type, name);
}

void PelicanServer::verbose(const QString& msg, int level)
{
     if( level < _verboseLevel )
        std::cout << msg.toStdString() << std::endl;
}

/**
 * @details
 * Adds a service chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
void PelicanServer::addServiceChunker(QString type, QString name)
{
    _chunkerManager->addServiceChunker(type, name);
}

/**
 * @details
 * Runs the server thread starting its event loop.
 *
 * Sets up the data manager which handles stream and service data buffers
 * which are set up on request of the chunkers.
 */
void PelicanServer::run()
{
    try {
        QVector<boost::shared_ptr<PelicanPortServer> > servers;

        // Set up the data manager.
        DataManager dataManager(_config);
        dataManager.setVerbosity(_verboseLevel);
        _chunkerManager->init(dataManager);

        // Set up listening servers.
        QList<quint16> ports = _protocolPortMap.keys();
        for (int i = 0; i < ports.size(); ++i) {
            boost::shared_ptr<PelicanPortServer> server(
                    new PelicanPortServer(_protocolPortMap[ports[i]], &dataManager) );
            server->setVerbosity(_verboseLevel);
            servers.append(server);
            if ( !server->listen(QHostAddress::Any, ports[i]) )
                throw QString("Cannot run PelicanServer on port %1").arg(ports[i]);
            verbose( QString("PelicanServer: listening on port %1").arg(ports[i]), 1 );
        }

        // Set ready flag.
        _mutex.lock();
        _ready = true;
        _mutex.unlock();

        // Enter the server's event loop.
        exec();
    }
    catch( QString& e ) {
        std::cerr << "PelicanServer caught an error: " << e.toStdString() << std::endl;
        verbose(QString("Shutting down server thread: ") + e);
        exit(1);
    }
}

void PelicanServer::setVerbosity(int level) {
     _verboseLevel = level;
}
} // namespace pelican
