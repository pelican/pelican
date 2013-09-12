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

#include "server/ServiceDataBuffer.h"

#include "server/LockedData.h"
#include "server/WritableData.h"
#include "server/LockableServiceData.h"

#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <stdlib.h>

namespace pelican {

/**
 * @details
 * Constructs the service data buffer.
 *
 * @param type         A string containing the type of data held in the buffer.
 * @param max          The maximum size of the buffer in bytes.
 * @param maxChunkSize The maximum chunk size in bytes.
 * @param parent       (Optional.) Pointer to the object's parent.
 */
ServiceDataBuffer::ServiceDataBuffer(const QString& type, const size_t max,
        const size_t maxChunkSize, QObject* parent)
: AbstractDataBuffer(type, parent)
{
    _max = max;
    _maxChunkSize = maxChunkSize;
    if (_maxChunkSize == 0) {
        _maxChunkSize = max;
    }
    _space = _max; // Buffer initially empty so space = max size.
    _newData = 0;
    _id = 0;
}

/**
 * @details
 * Destroys the service data buffer.
 */
ServiceDataBuffer::~ServiceDataBuffer()
{
    delete _newData;
    foreach (LockableServiceData* data, _data) {
        free(data->dataChunk()->data());
        delete data;
    }
}

/**
 * @details
 * Returns the specified version of the service data in the passed LockedData
 * object. If this data is unavailable then return an invalid.
 *
 * @param[in,out] lockedData  A reference to the LockedData object to set.
 * @param[in] version         The version identifier of the data to return.
 */
void ServiceDataBuffer::getData(LockedData& lockedData, const QString& version)
{
    QMutexLocker lock(&_mutex);
    lockedData.setData(_data.value(version));
}

/**
 * @details
 * Returns the current version of the service data in the passed LockedData
 * object.
 *
 * @param[in,out] lockedData  A reference to the LockedData object to set.
 */
void ServiceDataBuffer::getCurrent(LockedData& lockedData)
{
    QMutexLocker lock(&_mutex);
    lockedData.setData(_data.value(_current));
}

/**
 * @details
 * Gets a writable data object of the given size and returns it.
 * This object is locked, and on unlocking will become the current service data.
 *
 * @param[in] size The size of the writable data to return.
 */
WritableData ServiceDataBuffer::getWritable(size_t size)
{
    // XXX find out what determines if _newData is allocated...
    // --> something to do with activation of the data.
    // (activateData() method...)

    // If the temporary lockable service data chunk isn't allocated.
    // XXX what is this check for?
    if (!_newData)
    {
        QMutexLocker lock(&_mutex);

        // Check if any of the expired data chunks can be reused.
        for (int i = 0; i < _expiredData.size(); ++i)
        {
            LockableServiceData* lockableData = _expiredData[i];
            if (lockableData->maxSize() >= size)
            {
                // We found one, so reuse it and remove it from the
                // expired data queue.
                _data.remove(lockableData->id());
                _expiredData.removeAt(i);
                lockableData->setSize(size);
                return lockableData;
            }
        }

        // Create a new data object if we have enough space.
        if (size <= _space && size <= _maxChunkSize)
        {
            // Released in destructor.
            void* memory = calloc(size, sizeof(char));
            if (memory)
            {
                _space -= size;
                _newData = new LockableServiceData(_type, memory, size);
                connect(_newData, SIGNAL(unlockedWrite()), SLOT(activateData()));
                connect(_newData, SIGNAL(unlocked()), SLOT(deactivateData()));
                return WritableData(_newData);
            }
        }
    }

    // No free containers so we return an invalid
    // FIXME Clarify this return case.
    // It seems we get here if
    // 1) There is no memory left in the buffer.
    // 2) There is already an active LockableServiceData chunk.
    // NOTE: case (2) needs looking into more carefully.
    return WritableData(0);
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlockedWrite() signal. It calls the method to activate the data
 * chunk that emitted the signal.
 */
void ServiceDataBuffer::activateData()
{
    activateData(static_cast<LockableServiceData*>(sender()));
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlocked() signal. It calls the method to deactivate the data
 * chunk that emitted the signal.
 */
void ServiceDataBuffer::deactivateData()
{
    deactivateData(static_cast<LockableServiceData*>(sender()));
}

/**
 * @details
 * Deactivates the passed LockableServiceData object, and checks if it can
 * be expired.
 *
 * The data cannot be deleted as soon as there is no longer any stream data
 * referencing it, since a client may still ask for the data.
 *
 * If the passed data object is not the "current" service data (which is
 * associated with any incoming stream data), it is marked as available for
 * removal in the event of the buffer becoming full.
 *
 * @param[in] data Pointer to the LockableServiceData object to activate.
 */
void ServiceDataBuffer::deactivateData(LockableServiceData* data)
{
    if (data->id() != _current && ! _expiredData.contains(data)) {
        _expiredData.append(data);
    }
}

/**
 * @details
 * Activates the passed LockableServiceData object, making it available
 * to be served.
 *
 * @param[in] data Pointer to the LockableServiceData object to activate.
 */
void ServiceDataBuffer::activateData(LockableServiceData* data)
{
    if (data->isValid())
    {
        QMutexLocker lock(&_mutex);

        // Determine the version ID for this object.
        QString id = data->id();
        if (id.isEmpty()) {
            // Generate a unique ID if not already set.
            id.setNum(++_id);
            data->setId(id);
        }

        // Move from pending into the active data hash.
        _data.insert(id, data);
        _newData = 0;

        // Set the current pointer to this one.
        _current = id;
    }
}


size_t ServiceDataBuffer::usableSize(size_t chunkSize)
{
    // Number of chunks that fit in the remaining space
    size_t total = chunkSize > 0 ? (_space/chunkSize)*chunkSize : _space;
    QMutexLocker lock(&_mutex);
    foreach (LockableServiceData* s, _expiredData) {
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

size_t ServiceDataBuffer::usedSize()
{
    size_t total = 0;
    QMutexLocker lock(&_mutex);
    foreach (const LockableServiceData* s, _data) {
        total += s->dataChunk().get()->size();
    }
    return total;
}

int ServiceDataBuffer::numChunks() const
{
    return _data.size();
}

int ServiceDataBuffer::numEmptyChunks() const
{
    return _expiredData.size();
}

int ServiceDataBuffer::numUsableChunks(size_t chunkSize)
{
    Q_ASSERT(chunkSize > 0);
    int num = _space/chunkSize;
    QMutexLocker lock(&_mutex);
    foreach (const LockableServiceData* s, _expiredData) {
        if (s->maxSize() >= chunkSize)
            ++num;
    }
    return num;
}


} // namespace pelican
