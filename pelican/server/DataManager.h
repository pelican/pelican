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

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

/**
 * @file DataManager.h
 */

#include <QtCore/QString>
#include <QtCore/QHash>
#include "server/StreamDataBuffer.h"
#include "server/ServiceDataBuffer.h"
#include "server/LockedData.h"
#include "data/DataSpec.h"
#include "utility/Config.h"

namespace pelican {

class DataChunk;
class StreamData;

/**
 * @ingroup c_server
 *
 * @class DataManager
 *
 * @brief
 * Main class for accessing data in the server.
 *
 * @details
 * Class to control access to the internal server data to ensure
 * locking etc.
 *
 * Configuration:
 * Each stream has its own section with the tag of the stream name.
 * Buffer sizes are set with the  buffer tags
 * e.g.
 *
 * <MyStream>
 *      <buffer maxSize="10240"> tags
 * </MyStream>
 */
class DataManager
{
    private:
        QHash<QString, StreamDataBuffer*> _streams;
        QHash<QString, ServiceDataBuffer*> _service;
        QHash<StreamDataBuffer*, QString> _deactivate;
        DataSpec _specs;

    public:
        /// DataManager constructor.
        DataManager(const Config* config, const QString section = QString("server"));
        DataManager(const Config* config, const Config::TreeAddress& base );

        /// set verbosity level (0 = off)
        void setVerbosity(int level) { _verboseLevel = level; };

        /// DataManager destructor.
        virtual ~DataManager();

        /// Associate service data
        void associateServiceData(LockableStreamData* data);

        /// add to the  list of default adapters
        void addDefaultAdapters( const QHash<QString,QString>& );

        /// Returns the data types handled by this manager.
        const DataSpec& dataSpec() const {return _specs;}

        /// Return a list of Stream Data objects corresponding
        //  to a DataSpec object
        QList<LockedData> getDataRequirements(const DataSpec& req);

        /// Return the next unlocked data block from Stream Data.
        /// If the associate data requested is unavailable,
        /// LockedData will be invalid.
        LockedData getNext(const QString& type, const QSet<QString>& associateData);

        /// Return the next unlocked data block from Stream Data.
        LockedData getNext(const QString& type);

        /// Return the requested Service Data.
        LockedData getServiceData(const QString& type, const QString& version);

        /// Set up a stream buffer for the specified type.
        StreamDataBuffer* getStreamBuffer(const QString& type);

        /// Set up a service buffer for the specified type.
        ServiceDataBuffer* getServiceBuffer(const QString& type);

        /// Return a WritableData object that represents
        /// a space in the buffer of a minimum size specified
        /// An invalid Writable object will be returned if
        /// the space is not available.
        WritableData getWritableData(const QString& type, size_t size);

        /// Add a service data type to be managed.
        void setServiceDataBuffer(const QString& name, ServiceDataBuffer* buffer);

        /// Add a stream data type to be managed.
        void setStreamDataBuffer(const QString& name, StreamDataBuffer* buffer);

        /// register the specified data stream for deactivation
        void deactivateStream(const QString& name);

        /// indicate that a stream buffer is empty
        //  to be called by the streambuffer only
        void emptiedBuffer(StreamDataBuffer* buffer);

        /// set the max buffer size to be used for any new buffers
        //  to be created of the specified stream
        void setMaxBufferSize( const QString& stream, size_t size );
        /// set the max chunk size to be used for any new buffers
        //  to be created of the specified stream
        void setMaxChunkSize( const QString& stream, size_t size );

    protected:
        void verbose( const QString& msg, int verboseLevel = 1 );

    public:
        const Config* _config;
        Config::TreeAddress _bufferConfigBaseAddress;

    private:
        QHash<QString,size_t> _bufferMaxSizes;
        QHash<QString,size_t> _bufferMaxChunkSizes;
        int _verboseLevel;
};

} // namespace pelican

#endif // DATA_MANAGER_H
