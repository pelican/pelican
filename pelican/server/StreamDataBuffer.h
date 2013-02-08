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

#ifndef STREAMDATABUFFER_H
#define STREAMDATABUFFER_H

/**
 * @file StreamDataBuffer.h
 */

#include "server/AbstractDataBuffer.h"
#include <QtCore/QQueue>
#include <QtCore/QObject>

namespace pelican {

class LockableStreamData;
class DataChunk;
class DataManager;
class LockedData;
class WritableData;

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
 */
class StreamDataBuffer : public AbstractDataBuffer
{
    Q_OBJECT

    private:
        friend class StreamDataBufferTest;

    public:
        /// Constructs a stream data buffer.
        StreamDataBuffer(const QString& type,
                         const size_t max = 10240,
                         const size_t maxChunkSize = 10240,
                         QObject* parent = 0);

        /// Destroys the stream data buffer.
        ~StreamDataBuffer();

        /// Get the next data object that is ready to be served.
        void getNext(LockedData&);

        /// Get a data object that is ready to be written to.
        WritableData getWritable(size_t size);

        /// Set the data manager to use.
        void setDataManager(DataManager* manager) {_manager = manager;}

        /// Sets the maximum size of the buffer in bytes.
        void setMaxSize(const size_t size) { _max = size; }
        size_t size() const { return _max; }

        /// Sets the maximum chunk size of the in bytes.
        void setMaxChunkSize(const size_t size) { _maxChunkSize = size; }

        /// get the number of chunks waiting on the serve queue
        int numberOfActiveChunks() const;

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
        StreamDataBuffer(const StreamDataBuffer&); // Disallow copying.

    private:
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        QList<LockableStreamData*> _data; ///< List of all allocated memory blocks.
        QQueue<LockableStreamData*> _serveQueue; ///< Queue of blocks waiting to be served.
        QList<LockableStreamData*> _emptyQueue; ///< List of all available blocks.
        DataManager* _manager;
};

} // namespace pelican

#endif // STREAMDATABUFFER_H
