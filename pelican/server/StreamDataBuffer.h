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
 * Container class for chunks of StreamData.
 *
 * @details
 *
 * Encapsulates memory allocation for streams, with locking and data
 * consistency checking.
 *
 * StreamData is data that can be used only once, and once used is discarded
 * (expired / deactivated).
 *
 * The buffer consists of chunks of data whose size is determined by
 * requested size at first allocation and limited to the maximum chunk size
 * attribute of the buffer. The total size of the buffer is also limited by
 * the maximum buffer size attribute.
 * Note that as a result of this chunks in the buffer can be of mixed size.
 *
 *
 * Chunks of data in the buffer can be in one of three states:
 * - Locked    = The state of data chunks that are currently being accessed.
 *   (in use)    Access can is either in the form of being written to,
 *               e.g. inside a Chunker, or being read from, e.g. when being
 *               sent by a server.
 * - Active    = The state of data chunks which are ready to be used/ served.
 *   (usable)    These data chunks contain current data that is ready to be
 *               used or served.
 * - Expired   = Data chunks marked as already having been used and are ready
 *   (empty)     for reuse.
 *
 *
 * Chunks of data in the buffer are created using the @p getWriable() method
 * and returned inside a WriableData container. While under control of
 * the WriableData object, data chunks are in the Locked sate. The WriableData
 * becomes unlocked, and active, on destruction.
 *
 * Chunks of data in the buffer are accessed for use by using the @p getNext()
 * method. This returns the oldest active chunk in the buffer inside a
 * LockedData container. The LockedData container automatically handles
 * locking and unlocking of the data it contains. The data is locked upon
 * construction and unlocked upon destruction. When unlocked data is marked
 * as expired and the memory is then listed for reuse.
 *
 * When asking for a writable chunk of data, memory in the buffer is allocated
 * on demand, using the following priority.
 *
 *  1) If an expired (empty) chunk meets the size requirement passed when
 *     asking for the chunk it is reused.
 *  2) Otherwise, if there is sufficient space in the buffer a new chunk is
 *     allocated of the requested size.
 *  3) If conditions 1 and 2 can't be met, the oldest active chunk which meets
 *     the size requirement is removed and reused.
 *  4) Finally if no chunk can be obtained a null, invalid chunk is returned.
 *
 *
 * On creation of the Locked state WriableData object it is associated with the
 * current version of any ServiceData registered in buffers managed by the
 * associated DataManager object.
 *
 * ----------------------------------------------------------------------------
 * FIXME should buffers have a **name** as well as the type?
 * FIXME Need for DataManager breaks encapsulation?
 *       - options:
 *          * If there are no associated DataManagers ignore service data
 *            association and don't inform the manager of emptied buffers.
 *          * Data managers handle the association and emptied status themselves
 *            so the buffer dosn't need to know about it.
 *            > This would mean chunkers would be calling getWriable()
 *              on the DataManager rather than the buffer.
 *            > ... and whoever calls getNext would do it also via DataManager.
 *
 * FIXME Make overwrite of old chunks when asking for wriableData a buffer mode
 *       which is default enabled.
 */
class StreamDataBuffer : public AbstractDataBuffer
{
    private:
        Q_OBJECT
        friend class StreamDataBufferTest;

    public:
        /// Constructs a stream data buffer. If @p bufferSizeMax and/or
        /// @p chunkSizeMax are not specified they are defaulted to 10240 bytes.
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
        void setDataManager(DataManager* manager) { _dataManager = manager; }

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
        DataManager* _dataManager;
};

} // namespace pelican

#endif // PELICAN_STREAM_DATA_BUFFER_H
