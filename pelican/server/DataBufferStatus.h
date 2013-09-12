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

#ifndef PELICAN_DATA_BUFFER_STATUS_H_
#define PELICAN_DATA_BUFFER_STATUS_H_

#include <vector>

// XXX
//
// Instead of this class have the DataBuffers (or AbstractDataBuffer)
// hold these stats and return that from the DataManager and or AbstractChunker
//

class DataBufferStatus
{

public:
    DataBufferStatus()
    : bufferSizeMax_(0), chunkSizeAllowedMax_(0), allocatedSize_(0),
      inUseSize_(0), numTotalChunks_(0), numActiveChunks_(0),
      numExpiredChunks_(0)
    {}

    ~DataBufferStatus() {}

    // Converts bytes to MiB
    static inline double bytesToMiB(size_t bytes) const
    { return bytes/(1024.0*1024.0); }

    size_t maxSize() const { return bufferSizeMax_; }
    size_t maxAllowedChunkSize() const { return chunkSizeAllowedMax_; }
    size_t allocatedSize() const { return allocatedSize_; }
    size_t inUseSize() const { return inUseSize_; }
    unsigned int numTotalChunks() const { return numTotalChunks_; }
    unsigned int numActiveChunks() const { return numActiveChunks_; }
    unsigned int numExpiredChunks() const { return numExpiredChunks_; }
    size_t maxChunkSize(unsigned int chunkIndex)
    { return maxChunkSize_[chunkIndex]; }
    size_t currentChunkSize(unsigned int chunkIndex)
    { return currentChunkSize_[chunkIndex]; }


protected: // These should not be public to classes just querying the buffer
           // status
           // Rather than being protected could just make sure people don't
           // have access to a non-const DataBufferStatus object ?

    void setMaxSize(size_t value) { bufferSizeMax_ = value; }
    void setMaxAllowedChunkSize(size_t value) { chunkSizeAllowedMax_ = value; }
    void setAllocatedSize(size_t value) { allocatedSize_ = value; }
    void setInUseSize(size_t value) { inUseSize_ = value; }
    void setTotalChunks(unsigned int value) { numTotalChunks_ = value; }
    void setActiveChunks(unsigned int value) { numTotalChunks_ = value; }
    void setExpiredChunks(unsigned int value) { numTotalChunks_ = value; }
    void setMaxChunkSize(unsigned int chunkIndex, size_t value)
    { maxChunkSize_[chunkIndex] = value; }
    void setCurrentSize(unsigned int chunkIndex, size_t value)
    { currentChunkSize_[chunkIndex] = value; }

private:
    size_t bufferSizeMax_;           // Maximum buffer size, in bytes
    size_t chunkSizeAllowedMax_;    // Maximum allowed chunk size, in bytes.
    size_t allocatedSize_;           // Current buffer size allocated, in bytes
    size_t inUseSize_;               // Amount of memory in use in the buffer.
    unsigned int numTotalChunks_;    // Total number of chunks
    unsigned int numActiveChunks_;   // Number of chunks in the serve queue
    unsigned int numExpiredChunks_;  // Number of chunks marked for reuse.
    std::vector<size_t> maxChunkSize_;     // The maximum size of each chunk, in bytes.
    std::vector<size_t> currentChunkSize_; // The current size of each chunk, in bytes.
};

#endif /* PELICAN_DATA_BUFFER_STATUS_H_ */
