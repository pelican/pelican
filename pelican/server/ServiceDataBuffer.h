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

#ifndef SERVICEDATABUFFER_H
#define SERVICEDATABUFFER_H

#include "server/AbstractDataBuffer.h"

#include <QtCore/QObject>
#include <QtCore/QHash>

/**
 * @file ServiceDataBuffer.h
 */

namespace pelican {

class LockableServiceData;
class LockedData;

/**
 * @ingroup c_server
 *
 * @class ServiceDataBuffer
 *
 * @brief
 * Maintains service data.
 *
 * @details
 * Access to service data is assumed to be by direct reference,
 * either through an explicit identifier, or through the current()
 * method.
 * Multiple threads may access the same data at the same time for
 * reading.
 */
class ServiceDataBuffer : public AbstractDataBuffer
{
    private:
        Q_OBJECT
        friend class ServiceDataBufferTest;

    public:
        /// Constructs a service data buffer.
        ServiceDataBuffer(const QString& type, const size_t max = 10240,
                const size_t maxChunkSize = 10240, QObject* parent = 0);

        /// Destroys the service data buffer.
        ~ServiceDataBuffer();

        /// Return the specified version of the service data.
        void getData(LockedData& lockedData, const QString& version);

        /// Return the current version of the service data.
        void getCurrent(LockedData& lockedData);

        /// Returns a section of writable memory to be filled.
        WritableData getWritable(size_t size);

        /// Returns the maximum buffer size, in bytes.
        size_t maxSize() const { return _max; }

        /// Returns the maximum chunk size, in bytes.
        size_t maxChunkSize() const { return _maxChunkSize; }

        /// Returns the unallocated space in the buffer, in bytes.
        size_t space() const { return _space; }

        /// Returns the number of bytes of free space that can be used
        /// for chunks of the specified size.
        size_t usableSize(size_t chunkSize);

        /// Returns the number of bytes of memory in use in the buffer.
        size_t usedSize();

        /// Returns the total number of chunks in the buffer.
        int numChunks() const;

        /// Returns the number of empty or expired chunks in the buffer.
        int numEmptyChunks() const;

        /// Returns the number of usable chunks in the buffer that are large
        /// enough for the specified chunk size.
        int numUsableChunks(size_t chunkSize);

    protected slots:
        void activateData();
        void deactivateData();

    protected:
        void activateData(LockableServiceData*);
        void deactivateData(LockableServiceData*);

    private:
        size_t _max;           // Maximum buffer size, in bytes.
        size_t _maxChunkSize;  // Maximum allowed chunk size, in bytes.
        size_t _space;         // Current free (unallocated) space, in bytes.

        LockableServiceData* _newData; // Temporary store until activated

        QHash<QString, LockableServiceData*> _data; // All allocated memory blocks.
        QList<LockableServiceData*> _expiredData;   // Expired memory blocks ready for reuse.

        QString _current;     // The current server data block version label
        unsigned long _id;    // FIXME what exactly is this index ???
};

} // namespace pelican

#endif // SERVICEDATABUFFER_H
