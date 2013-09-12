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

#ifndef PELICAN_STREAM_DATA_BUFFER_H
#define PELICAN_STREAM_DATA_BUFFER_H

/**
 * @file StreamDataBuffer.h
 */

#include "server/AbstractDataBuffer.h"
#include "server/WritableData.h"

#include <QtCore/QString>
#include <QtCore/QQueue>
#include <QtCore/QObject>

#include <cstdio> // for size_t

namespace pelican {

class LockableStreamData;
class DataChunk;
class DataManager;
class LockedData;


/**
 * @ingroup c_server
 *
 * @class StreamDataBuffer
 *
 * @brief
 * Maintain StreamData and memory access.
 *
 * @details
 * Encapsulates memory allocation for streams, with locking and data
 * consistency checking.
 *
 * Note the default maximum allowed size of the buffer is 10240 bytes
 * and the maximum allowed chunk size in the buffer is also 10240 bytes.
 *
 * FIXME should buffers have a name as well as the type?
 * FIXME Need for DataManager breaks encapsulation?
 *
 */
class StreamDataBuffer : public AbstractDataBuffer
{
    private:
        Q_OBJECT
        friend class StreamDataBufferTest;

    public:
        /// Constructs a stream data buffer.
        // FIXME is there any case where there is a valid use of passing parent?
        StreamDataBuffer(const QString& type, size_t bufferSizeMax = 10240,
                size_t chunkSizeMax = 10240, QObject* parent = 0);

        /// Destroys the stream data buffer.
        ~StreamDataBuffer();

        /// Get a data object that is ready to be written to.
        // REFACTOR to getChunk() ?
        WritableData getWritable(size_t size);

        /// Get the next data object that is ready to be served.
        void getNext(LockedData&);

        /// Set the data manager to use.
        void setDataManager(DataManager* manager) { _manager = manager; }

        /// Returns the maximum size of the buffer, in bytes.
        // DEPRECATED in buffer status function re-write
        size_t maxSize() const { return _max; }

        /// Returns the maximum allowed chunk size, in bytes.
        // DEPRECATED in buffer status function re-write
        size_t maxChunkSize() const { return _maxChunkSize; }

#if 0
        // DEPRECATED
        void setMaxSize(size_t value) { _max = value; }

        // DEPRECATED
        void setMaxChunkSize(size_t value) { _maxChunkSize = value; }
#endif

        /// get the number of chunks waiting on the serve queue
        // DEPRECATED in buffer status function re-write
        int numberOfActiveChunks() const;

        // DEPRECATED in buffer status function re-write
        size_t numberOfEmptyChunks() const;

        /// Returns the amount of unallocated space in the buffer.
        // DEPRECATED in buffer status function re-write
        size_t space() const { return _space; }

        /// Returns the number of bytes allocated in the buffer.
        // DEPRECATED in buffer status function re-write
        size_t allocatedBytes() const;

        /// Returns the number of bytes of free space that can be used
        /// for chunks of the specified size.
        // DEPRECATED in buffer status function re-write
        size_t usableSize(size_t chunkSize);

        /// Returns the number of bytes of memory in use in the buffer.
        // DEPRECATED in buffer status function re-write
        size_t usedSize();

        /// Returns the total number of chunks allocated in the buffer.
        // DEPRECATED in buffer status function re-write
        int numChunks() const;

        /// Returns the number of chunks that can be used for the given
        /// specified chunk size.
        // DEPRECATED in buffer status function re-write
        int numUsableChunks(size_t chunkSize);

    protected slots:
        /// Places the data chunk that emitted the signal on the serve queue.
        void activateData();

        /// Places the data chunk that emitted the signal on the empty queue.
        void deactivateData();

    protected:
        /// Places the given data chunk on the serve queue.
        void activateData(LockableStreamData*);

        /// Places the given data chunk on the empty queue.
        void deactivateData(LockableStreamData*);

        LockableStreamData* _getWritable(size_t size);

    private:
        // Disallow copying.
        StreamDataBuffer(const StreamDataBuffer&);

    private:
        size_t _max;           // Maximum buffer size, in bytes.
        size_t _maxChunkSize;  // Maximum allowed chunk size, in bytes.
        size_t _space;         // Current free (unallocated) space, in bytes.

        QList<LockableStreamData*>  _allChunks;  // All allocated memory blocks.
        QQueue<LockableStreamData*> _serveQueue; // Blocks waiting to be served.
        QList<LockableStreamData*>  _emptyQueue; // Blocks ready for reuse.

        // NOTE Currently the stream data buffer has to know about the
        // data manager so it can associate chunks with service data and
        // inform the data manager that the serve queue is empty when
        // all chunks have been deactivated.
        // There may be a cleaner way of doing this with better encapsulation.
        DataManager* _manager;
};

} // namespace pelican

#endif // PELICAN_STREAM_DATA_BUFFER_H
