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

#include "server/WritableData.h"
#include "server/LockedData.h"
#include "data/DataSpec.h"
#include "utility/Config.h"

namespace pelican {

class DataChunk;
class StreamData;
class LockableStreamData;
class ServiceDataBuffer;
class StreamDataBuffer;

/**
 * @ingroup c_server
 *
 * @class DataManager
 *
 * @brief
 * Main class for accessing data in the server.
 *
 * @details
 * Class to control access to the internal server data to ensure locking etc.
 *
 * This class has ownership of stream and service data buffers and will
 * delete them upon destruction.
 *
 * Configuration:
 * Each stream has its own section with the tag of the stream name.
 * Buffer sizes are set with the buffer tags:
 * e.g.
 * <MyStream>
 *    <buffer maxSize="10240">
 * </MyStream>
 */
class DataManager
{
    public:
        /// Constructor.
        DataManager(const Config* config, const QString section = "server");

        /// Constructor.
        DataManager(const Config* config, const Config::TreeAddress& base);

        /// Destructor.
        virtual ~DataManager();

        /// Add to the list of default adapters
        void addDefaultAdapters(const QHash<QString, QString>& types);

        /// Associate service data
        void associateServiceData(LockableStreamData* data);

        /// Returns the data types handled by this manager.
        const DataSpec& dataSpec() const { return _specs; }

        /// Register the specified data stream for deactivation.
        void deactivateStream(const QString& name);

        /// Indicate that a stream buffer is empty.
        /// To be called by the stream buffer only.
        void emptiedBuffer(StreamDataBuffer* buffer);

        /// Return a list of Stream Data objects corresponding to a DataSpec
        /// object.
        QList<LockedData> getDataRequirements(const DataSpec& req);

        /// Return the next unlocked data block from Stream Data. If the
        /// associate data requested is unavailable, LockedData will be invalid.
        LockedData getNext(const QString& type, const QSet<QString>& associateData);

        /// Return the next unlocked data block from Stream Data.
        LockedData getNext(const QString& type);

        /// Set up a service buffer for the specified type.
        ServiceDataBuffer* getServiceBuffer(const QString& type);

        /// Return the requested Service Data.
        LockedData getServiceData(const QString& type, const QString& version);

        /// Set up a stream buffer for the specified type.
        StreamDataBuffer* getStreamBuffer(const QString& type);

        /// Return a WritableData object that represents a space in the buffer
        /// of a minimum size specified. An invalid Writable object will be
        /// returned if the space is not available.
        WritableData getWritableData(const QString& type, size_t size);

        /// Set the max buffer size to be used for any new buffers
        /// to be created of the specified stream.
        void setMaxBufferSize(const QString& stream, size_t size);

        /// Set the max chunk size to be used for any new buffers
        /// to be created of the specified stream.
        void setMaxChunkSize(const QString& stream, size_t size);

        /// Add a service data type to be managed.
        void setServiceDataBuffer(const QString& name, ServiceDataBuffer* buffer);

        /// Add a stream data type to be managed.
        void setStreamDataBuffer(const QString& name, StreamDataBuffer* buffer);

        /// Set verbosity level (0 = off)
        void setVerbosity(int level) { _verboseLevel = level; };


    public: // new functions for 1.0.4 that can be used to query the
            // state of buffers.

        /// Returns the total number buffers held by the data manager.
        int numBuffers() const;

        /// Returns the number of stream buffers held by the data manager.
        int numStreamBuffers() const;

        /// Returns the number of service buffers held by the data manager.
        int numServiceBuffers() const;

        /// Returns the number of deactivated stream buffers.
        int numDeactivatedBuffers() const;

        /// Returns true if the specified type is contained in a stream buffer.
        bool isStream(const QString& type) const;

        /// Returns true if the specified type is contained in a service buffer.
        bool isService(const QString& type) const;

        /// Returns the maximum size, in bytes, of the buffer storing the
        /// specified data type @p type.
        size_t maxSize(const QString& type) const;

        /// Returns the maximum chunk size, in bytes, of chunks in the buffer
        /// storing the specified data type @p type.
        size_t maxChunkSize(const QString& type) const;

        /// Returns the number of bytes allocated in the buffer storing
        /// the specified type @p type.
        size_t allocatedSize(const QString& type) const;

        /// Returns the amount of free space in the buffer, in bytes, including
        /// memory in deactivated chunks that can be used for the specified
        /// chunk size.
        size_t usableSize(const QString& type, size_t chunkSize = 0) const;

        /// Returns the amount memory in use in the buffer, in bytes.
        size_t usedSize(const QString& type) const;

        /// Returns the total number of chunks that have been allocated in the
        /// buffer of the specified type @p type.
        int numChunks(const QString& type) const;

        /// Returns the number of active chunks (chunks ready to be served) in
        /// the buffer of the specified type @p type
        int numActiveChunks(const QString& type) const;

        /// Returns the number of expired chunks (chunks that have been
        /// allocated and are no longer in use) in the buffer of the specified
        /// type @p type
        int numExpiredChunks(const QString& type) const;

        /// Returns the number of usable chunks (either unallocated or expired)
        /// in the buffer of the specified type @p type and large enough
        /// to store @p chunkSize bytes.
        int numUsableChunks(const QString& type, size_t chunkSize) const;

    protected:
        void verbose(const QString& msg, int verboseLevel = 1);

    private:
        const Config* _config;  // XML configuration.
        // Address of XML relating to data buffers.
        Config::TreeAddress _bufferConfigBaseAddress;
        // Set of stream and service data types.
        DataSpec _specs;
        QHash<QString, size_t> _bufferMaxChunkSizes;
        QHash<QString, size_t> _bufferMaxSizes;
        QHash<StreamDataBuffer*, QString> _deactivate;
        QHash<QString, StreamDataBuffer*> _streams;
        QHash<QString, ServiceDataBuffer*> _service;
        int _verboseLevel;
};

} // namespace pelican

#endif // DATA_MANAGER_H
