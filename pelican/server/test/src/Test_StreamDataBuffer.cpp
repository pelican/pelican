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

#include <gtest/gtest.h>

#include <server/StreamDataBuffer.h>

using pelican::StreamDataBuffer;
using pelican::WritableData;


TEST(StreamDataBuffer, stack_create)
{
    QString type = "foo";
    size_t maxSize = 10240;
    size_t maxChunkSize = 1024;
    StreamDataBuffer buffer(type, maxSize, maxChunkSize);

    ASSERT_EQ(maxSize, buffer.maxSize());
    ASSERT_EQ(maxChunkSize, buffer.maxChunkSize());
    ASSERT_EQ(type, buffer.type());
}


TEST(StreamDataBuffer, heap_create)
{
    QString type = "foo";
    size_t maxSize = 10240;
    size_t maxChunkSize = 1024;
    StreamDataBuffer* buffer = new StreamDataBuffer(type, maxSize, maxChunkSize);

    ASSERT_EQ(maxSize, buffer->maxSize());
    ASSERT_EQ(maxChunkSize, buffer->maxChunkSize());
    ASSERT_EQ(type, buffer->type());

    delete buffer;
}

TEST(StreamDataBuffer, getWriable)
{
    QString type = "foo";
    size_t maxSize = 10240;
    size_t maxChunkSize = 1024;
    StreamDataBuffer buffer(type, maxSize, maxChunkSize);

    // NOTE
    // For getWriable() to work the StreamDataBuffer needs a DataManager
    // options:
    //  - remove / relax this requirement
    //  - Create a DataManager here.
    //  - friend class the test and avoid methods that need DataManager.

    try {
        // [Use case]
        // Ask for a chunk of maxChunkSize bytes.
        // [Expect]
        // 1) A valid chunk.
        // 2) Buffer status to reflect existence of new chunk.
        // 3) When the chunk of Writable data goes out of scope it will be
        //    added to the serve queue.
        WritableData chunk = buffer.getWritable(maxChunkSize);
        ASSERT_TRUE(chunk.isValid());
        // FIXME the chunk should probably know its size and max size directly
        // rather than having to go via AbstractLockableData.
        ASSERT_EQ(maxChunkSize, chunk.data()->maxSize());
    }
    catch (const QString& error) {
        FAIL() << "Exception thrown: "<< error.toStdString();
    }


}

