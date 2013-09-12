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

#include "server/StreamDataBuffer.h"
#include "server/DataManager.h"
#include "server/LockableStreamData.h"
#include "server/LockedData.h"
#include "server/WritableData.h"
#include "comms/StreamData.h"

#include <QtCore/QMutexLocker>
#include <stdlib.h>

namespace pelican {

/**
 * @details
 * Constructs the stream data buffer.
 *
 * @param type         A string containing the type of data held in the buffer.
 * @param max          The maximum size of the buffer. in bytes.
 * @param maxChunkSize The maximum allowed chunk size, in bytes.
 * @param parent       (Optional) Pointer to the object's parent.
 */
StreamDataBuffer::StreamDataBuffer(const QString& type, size_t max,
        size_t maxChunkSize, QObject* parent)
: AbstractDataBuffer(type, parent), _max(max), _maxChunkSize(maxChunkSize),
  _space(max), _manager(0)
{
    Q_ASSERT(max > 0);

    // If the max chunkSize is set to zero, set it to the size of the buffer.
    if (maxChunkSize == 0)
        _maxChunkSize = max;
}


/**
 * @details
 * Destroys the stream data buffer, freeing the memory held by it.
 */
StreamDataBuffer::~StreamDataBuffer()
{
    foreach (LockableStreamData* lockedData, _allChunks) {
        // Must use free() as allocated with calloc()
        free(lockedData->dataChunk()->data());
        delete lockedData;
    }
}


/**
 * @details
 * Gets the next block of data to serve.
 */
void StreamDataBuffer::getNext(LockedData& lockedData)
{
    QMutexLocker locker(&_mutex);

    // Check if the serve queue is empty.
    if (_serveQueue.isEmpty()) {
        lockedData.setData(0); // Return an invalid data block.
        return;
    }

    // Remove the data from the serve queue.
    lockedData.setData(_serveQueue.dequeue());
}


/**
 * @details
 * Gets a writable data object of the given size and returns it.
 *
 * @param[in] requestedSize The size of the writable data to return.
 *
 * @return
 */
WritableData StreamDataBuffer::getWritable(size_t requestedSize)
{
    QMutexLocker locker(&_writeMutex);
    LockableStreamData* lockableStreamData = _getWritable(requestedSize);

    // Prepare the object for use by adding Service Data info.
    if (lockableStreamData)
    {
        lockableStreamData->reset(requestedSize);
        if (!_manager)
            throw QString("StreamDataBuffer::getWritable(): No data manager.");
        _manager->associateServiceData(lockableStreamData);
    }

    return WritableData(lockableStreamData);
}


/**
 * @details
 * Private method to get a writable block of memory of the given size.
 *
 * @return Returns a pointer to the LockableStreamData object to use.
 *
 * @param[in] requestedSize The size in bytes of the requested block of writable memory.
 */
LockableStreamData* StreamDataBuffer::_getWritable(size_t requestedSize)
{
    // Return a pre-allocated block from the empty queue, if one exists.
    for (int i = 0; i < _emptyQueue.size(); ++i)
    {
        LockableStreamData* lockableData = _emptyQueue[i];

        // We found one, so our work is done.
        if (lockableData->maxSize() >= requestedSize)
        {
            _emptyQueue.removeAt(i);
            return lockableData;
        }
    }

    // If there are no empty containers already available, create a new
    // data object (chunk) if we have enough space and the requested size
    // is less than the allowed maximum chunk size.
    if (requestedSize <= _space && requestedSize <= _maxChunkSize)
    {
        // Note: Memory for the chunk is released in destructor.
        void* memory = calloc(requestedSize, sizeof(char));
        if (memory)
        {
            _space -= requestedSize;
            LockableStreamData* lockableData =
                    new LockableStreamData(_type, memory, requestedSize);

            // Add to the list of all data chunks
            _allChunks.append(lockableData);

            // Connect signals to the created data chunk.
            connect(lockableData, SIGNAL(unlockedWrite()), SLOT(activateData()));
            connect(lockableData, SIGNAL(unlocked()), SLOT(deactivateData()));

            return lockableData;
        }
    }

    // If there are no free containers and no space left, remove the oldest
    // waiting data that fits the size requirements
    {
        // Lock down the server queue while we are looping over it.
        QMutexLocker locker(&_mutex);
        for (int i = 0; i < _serveQueue.size(); ++i)
        {
            LockableStreamData* d = _serveQueue[i];
            if (d->maxSize() >= requestedSize) {
                _serveQueue.removeAt(i);
                return d;
            }
        }
    }

    // All else fails so we return an invalid (null) pointer.
    return 0;
}


/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlockedWrite() signal. It calls the method to activate the data
 * chunk that emitted the signal, putting it onto the serve queue.
 */
void StreamDataBuffer::activateData()
{
    activateData(static_cast<LockableStreamData*>(sender()));
}


/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlocked() signal. It calls the method to deactivate the data
 * chunk that emitted the signal, putting it onto the empty queue.
 */
void StreamDataBuffer::deactivateData()
{
    deactivateData(static_cast<LockableStreamData*>(sender()));
}


/**
 * @details
 * Puts the specified chunk of data on the empty queue, which allows the
 * space it occupied to be reused.
 */
void StreamDataBuffer::deactivateData(LockableStreamData* data)
{
    {
        // Note: Make sure to release the following serveQueue mutex before
        // attempting to get the write mutex otherwise the data buffer
        // can become locked.
        QMutexLocker locker(&_mutex);
        if (!data->served()) {
            // Inserts data at the beginning of the list.
            _serveQueue.prepend(data);
            return;
        }
        data->reset(0); // FIXME is there any case where the size argument here
                        // isnt == 0 ?!
        if (_serveQueue.empty()) {
            _manager->emptiedBuffer(this);
        }
    }

    // Obtain a write locker and put the data onto the empty queue.
    QMutexLocker writeLocker(&_writeMutex);
    _emptyQueue.push_back(data);
}


/**
 * @details
 * Puts the specified chunk of data on the queue ready to be served.
 */
void StreamDataBuffer::activateData(LockableStreamData* data)
{
    // If the data is valid place it on the serve queue.
    if (data->isValid()) {
        verbose("activating data", 2);
        QMutexLocker locker(&_mutex);
        _serveQueue.enqueue(data);
    }
    // Otherwise place it on the empty queue
    // FIXME is this else action the correct behaviour?
    else {
        verbose("not activating data - invalid", 2);
        QMutexLocker writeLocker(&_writeMutex);
        _emptyQueue.push_back(data);
    }
}

/// DEPRECATED in buffer status function re-write
int StreamDataBuffer::numberOfActiveChunks() const
{
    return _serveQueue.size();
}

/// DEPRECATED in buffer status function re-write
size_t StreamDataBuffer::numberOfEmptyChunks() const
{
    return (size_t)_emptyQueue.size();
}

/// DEPRECATED in buffer status function re-write
size_t StreamDataBuffer::allocatedBytes() const
{
#if 0
    size_t total = 0;
    for (int i = 0; i < _allChunks.size(); ++i) {
        total += _allChunks[i]->dataChunk().get()->size();
    }
    return total;
#endif
    return _max - _space;
}

/// DEPRECATED in buffer status function re-write
size_t StreamDataBuffer::usableSize(size_t chunkSize)
{
    // Number of chunks that fit in the remaining space
    size_t total = chunkSize > 0 ? (_space/chunkSize)*chunkSize : _space;

    QMutexLocker writeLocker(&_writeMutex);
    foreach (const LockableStreamData* s, _emptyQueue) {
        size_t dataSize = s->maxSize();
        if (dataSize >= chunkSize) {
            // Yes, I'm adding chunk size here if the chunkSize > 0
            // as the usable space in the empty chunk, is the requested chunk size
            // not the total size of the empty chunk.
            total += chunkSize > 0 ? chunkSize : dataSize;
        }
    }
    return total;
}

/// DEPRECATED in buffer status function re-write
size_t StreamDataBuffer::usedSize()
{
    size_t total = 0;
    QMutexLocker writeLocker(&_writeMutex);
    foreach (const LockableStreamData* s, _serveQueue) {
        total += s->dataChunk().get()->size();
    }
    return total;
}

/// DEPRECATED in buffer status function re-write
int StreamDataBuffer::numChunks() const
{
    return _allChunks.size();
}

/// DEPRECATED in buffer status function re-write
int StreamDataBuffer::numUsableChunks(size_t chunkSize)
{
    Q_ASSERT(chunkSize > 0);
    int num = _space/chunkSize;
    QMutexLocker writeLocker(&_writeMutex);
    foreach (const LockableStreamData* s, _emptyQueue) {
        if (s->maxSize() >= chunkSize)
            ++num;
    }
    return num;
}

} // namespace pelican
