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

#include "server/AbstractChunker.h"
#include "utility/ConfigNode.h"
#include "server/StreamDataBuffer.h"
#include "server/ServiceDataBuffer.h"

#include <iostream>

namespace pelican {

/**
 * @details
 * Setup options in the configuration file:
 *
 * listen to an incoming port
 *   <connection host="dataHost" port="12345" />
 *
 * To set the stream name for the chunker
 *   <data type="streamName" />
 *
 * To set a default adapter for the specific stream
 *   <data type="streamName" adapter="AdapterType" />
 */
AbstractChunker::AbstractChunker(const ConfigNode& config)
{
    // Initialise members.
    _dataManager = 0;

    _chunkTypes = config.getOptionList("data", "type");
    // NOTE Why do chunkers need to know adapter types... ?
    _adapterTypes = config.getOptionHash("data", "type", "adapter");
    // NOTE host and port is only valid for socket chunkers, sort out what
    // happens for file chunkers.
    _host = config.getOption("connection", "host", "");
    _port = (quint16)config.getOption("connection", "port", "0").toUInt();

    _active = true; // XXX is this right?
}


/**
 * @details
 * Destroys the AbstractChunker.
 */
AbstractChunker::~AbstractChunker()
{
    stop();
}

void AbstractChunker::setDataManager(DataManager* dataManager)
{
    _dataManager = dataManager;
    dataManager->addDefaultAdapters(defaultAdapters());
}

/**
 * @details
 * Returns a writable data object of the specified \p size.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
 *
 * @param[in] size  The size of the chunk requested on the buffer.
 */
WritableData AbstractChunker::getDataStorage(size_t size) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");

    if (_chunkTypes.size() != 1)
        throw QString("AbstractChunker::getDataStorage(): "
                "More than one chunk type registered, ambiguous request.");

    return _dataManager->getWritableData(_chunkTypes[0], size);
}


/**
 * @details
 * Returns a writable data object of the specified \p size and \p type.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
 *
 * @param[in] size  The size of the chunk requested on the buffer.
 * @param[in] type  The type (name) of the chunk in the data buffer.
 */
WritableData AbstractChunker::getDataStorage(size_t size,
        const QString& chunkType) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");

    return _dataManager->getWritableData(chunkType, size);
}

void AbstractChunker::setDefaultAdapter( const QString& adapter ) {
    if (_chunkTypes.size() != 1)
        throw QString("AbstractChunker::setDefaultAdapter(): "
                "More than one chunk type registered, ambiguous request.");
    setDefaultAdapter( adapter, _chunkTypes[0] );
}

void AbstractChunker::setDefaultAdapter( const QString& adapter,
        const QString& stream )
{
    _adapterTypes.insert(stream,adapter);
}


bool AbstractChunker::isStream(const QString type) const
{
    return _dataManager->isStream(type.isNull()?_chunkTypes[0]:type);
}

bool AbstractChunker::isService(const QString type) const
{
    return _dataManager->isService(type.isNull()?_chunkTypes[0]:type);
}

size_t AbstractChunker::maxBufferSize(const QString type) const
{
    return _dataManager->maxSize(type.isNull()?_chunkTypes[0]:type);
}

size_t AbstractChunker::maxChunkSize(const QString type) const
{
    return _dataManager->maxChunkSize(type.isNull()?_chunkTypes[0]:type);
}

size_t AbstractChunker::allocatedSize(const QString type) const
{
    return _dataManager->allocatedSize(type.isNull()?_chunkTypes[0]:type);
}

size_t AbstractChunker::usableSize(size_t size, const QString type) const
{
    return _dataManager->usableSize(type.isNull()?_chunkTypes[0]:type, size);
}

size_t AbstractChunker::usedSize(const QString type) const
{
    return _dataManager->usedSize(type.isNull()?_chunkTypes[0]:type);
}

int AbstractChunker::numChunks(const QString type) const
{
    return _dataManager->numChunks(type.isNull()?_chunkTypes[0]:type);
}

int AbstractChunker::numActiveChunks(const QString type) const
{
    return _dataManager->numActiveChunks(type.isNull()?_chunkTypes[0]:type);
}

int AbstractChunker::numExpiredChunks(const QString type) const
{
    return _dataManager->numExpiredChunks(type.isNull()?_chunkTypes[0]:type);
}

int AbstractChunker::numUsableChunks(size_t size, const QString type) const
{
    return _dataManager->numUsableChunks(type.isNull()?_chunkTypes[0]:type, size);
}

} // namespace pelican
