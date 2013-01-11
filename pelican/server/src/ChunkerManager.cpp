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

#include "server/ChunkerManager.h"
#include "server/DataReceiver.h"
#include "server/DataManager.h"
#include "server/AbstractChunker.h"

#include <QtCore/QVector>
#include <boost/shared_ptr.hpp>

// Include default chunkers here to ensure they are available in the factory.
#include "server/FileChunker.h"

#include <iostream>

namespace pelican {

/**
 * @details
 * Constructs the ChunkerManager, creating the chucker factory.
 */
ChunkerManager::ChunkerManager(const Config* config, const QString& section)
{
    // Create the chunker factory.
    _factory = new FactoryConfig<AbstractChunker>(config, section, "chunkers");
}


/**
 * @details
 * Constructs the ChunkerManager, creating the chucker factory.
 */
ChunkerManager::ChunkerManager(const Config* config,
        const Config::TreeAddress& base)
{
    // Create the chunker factory.
    Config::TreeAddress chunkerBase = base;
    chunkerBase << Config::NodeId("chunkers","");

    _factory = new FactoryConfig<AbstractChunker>(config, chunkerBase);
}


/**
 * @details
 * Destroys the ChunkerManager and the chunker factory it contains.
 */
ChunkerManager::~ChunkerManager()
{
    // Iterate over all data receivers and stop them.
    foreach (DataReceiver* receiver, _dataReceivers) {
        delete receiver;
    }

    // Delete the chunker factory.
    delete _factory;
}


/**
 * @details
 * Initialises the registered chunkers and calls the start() method on each
 * data receiver.
 */
bool ChunkerManager::init(DataManager& dataManager)
{
    // Create the stream and service data buffers.
    foreach (const QString& type, _streamDataTypes) {
        dataManager.getStreamBuffer(type);
    }

    foreach (const QString& type, _serviceDataTypes) {
        dataManager.getServiceBuffer(type);
    }

    // Set up data-stream inputs.
    try {
        foreach (AbstractChunker* chunker, _chunkers ) {
            chunker->setDataManager(&dataManager);
            DataReceiver* receiver = new DataReceiver(chunker);
            _dataReceivers.insert( chunker, receiver);
            receiver->start();
        }
    }
    catch( const QString& msg )
    {
        std::cerr << "Error Initiating Chunkers: " << msg.toStdString() << std::endl;
        return false;
    }
    return true;
}

bool ChunkerManager::isRunning() const
{
    if( _dataReceivers.size() == 0 )
        return false;

    bool rv = true;
    foreach (DataReceiver* dr, _dataReceivers.values() ) {
         rv &= dr->isRunning();
    }
    return rv;
}

/**
 * @details
 * Adds a stream chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
AbstractChunker* ChunkerManager::addStreamChunker(QString chunkerType, QString name)
{
    Q_ASSERT(chunkerType != "");

    // Create a chunker of the specified type with the factory.
    AbstractChunker* chunker = _factory->create(chunkerType, name);

    // If no data chunk types are registered to the chunker, set the chunker
    // data type to the chunker name.
    // NOTE: This should probably never happen!
    if (chunker->chunkTypes().empty())
    {
        std::cerr << "ChunkerManager::addStreamChunker(): WARNING Chunker '"
             << chunkerType.toStdString() << "' has no registered chunk data types."
             << std::endl;
        chunker->addChunkType(chunkerType);
    }

    // Add the chunker to the chunker manager.
    _addChunker(chunker);

    // Loop over the list of chunk types written into the data buffer
    // by the chunker and add them to the list of stream data types.
    foreach( const QString& chunkType, chunker->chunkTypes())
        _streamDataTypes.insert(chunkType);

    return chunker;
}


/**
 * @details
 * Adds a service chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
AbstractChunker* ChunkerManager::addServiceChunker(QString type, QString name)
{
    Q_ASSERT(type != "" );

    // Create a chunker of the specified type with the factory.
    AbstractChunker* chunker = _factory->create(type, name);

    // If no types are registered to the chunker set the chunker data type
    // to the chunker name.
    if (chunker->chunkTypes().empty())
    {
        std::cerr << "ChunkerManager::addStreamChunker(): WARNING Chunker '"
             << type.toStdString() << "' has no registered chunk data types."
             << std::endl;
        chunker->addChunkType(type);
    }

    // Add the chunker to the chunker manager.
    _addChunker(chunker);

    // Loop over the list of chunk types written into the data buffer
    // by the chunker and add them to the list of service data types.
    foreach (QString chunkType, chunker->chunkTypes())
        _serviceDataTypes.insert(chunkType);

    return chunker;
}

QIODevice* ChunkerManager::getCurrentDevice( AbstractChunker* chunker ) const
{
   if( _dataReceivers.contains( chunker ) ) {
       return _dataReceivers[chunker]->currentDevice();
   }
   return 0;
}

/**
 * @details
 * Adds the allocated chunker to the map of known chunkers.
 *
 * @param[in] chunker  Pointer to the allocated chunker.
 */
void ChunkerManager::_addChunker(AbstractChunker* chunker)
{
    if (chunker)
    {
        QPair<QString, quint16> pair(chunker->host(), chunker->port());

        // Check to see any chunkers are already assigned to the same host
        // and port.
        if (!chunker->host().isEmpty())
        {
            if (_chunkerPortMap.contains(pair)) {
                QString err = "ChunkerManager::_addChunker(): ";
                err += "Cannot map multiple chunkers to the same host/ port. ";
                err += "(host %1, port %2 already assigned)";
                err = err.arg(chunker->host());
                err = err.arg(chunker->port());
                throw err;
            }
            _chunkerPortMap[pair] = chunker;
        }
        _chunkers.insert(chunker);
    }
}

} // namespace pelican
