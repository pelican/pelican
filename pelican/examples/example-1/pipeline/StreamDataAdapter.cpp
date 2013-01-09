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

#include "StreamDataAdapter.hpp"
#include "StreamData.hpp"
#include <iostream>

using namespace std;

StreamDataAdapter::StreamDataAdapter(const pelican::ConfigNode& config)
: AbstractStreamAdapter(config)
{
    samplesPerPacket_ = config.getOption("packet", "numSamples").toUInt();
    packetSize_ = headerSize_ + (samplesPerPacket_ * sizeof(float));

    Q_ASSERT(samplesPerPacket_ != 0);
}

void StreamDataAdapter::deserialise(QIODevice* device)
{
    StreamData* blob = (StreamData*) dataBlob();
    unsigned packets = chunkSize() / packetSize_;
    Q_ASSERT(packets >= 1);
    blob->resize(packets * samplesPerPacket_);

    char headerData[headerSize_];
    char* data = (char*)blob->ptr();

    int bytesRead = 0;
    for (unsigned p = 0; p < packets; ++p)
    {
        while (device->bytesAvailable() < packetSize_)
            device->waitForReadyRead(-1);

        // Read the packet header from the device and dump it.
        device->read(headerData, headerSize_);

        // Read the data into the data blob.
        bytesRead += device->read(data + bytesRead, packetSize_ - headerSize_);
    }
}
