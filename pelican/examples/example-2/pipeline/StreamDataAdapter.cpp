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
}

void StreamDataAdapter::deserialise(QIODevice* device)
{
    size_t bytesRead = 0;
    size_t totalBytesRead = 0;

    // Header values
    quint32 packetSize = 0;
    quint32 packetCounter = 0;
    quint32 packetTime = 0;
    qint32 numPackets = 0;
    qint32 reportInterval = 0;
    size_t headerSize = 3*sizeof(quint32)+2*sizeof(qint32);

    // Read the header.
    while (totalBytesRead != headerSize) {
        while (device->bytesAvailable() < (qint64)headerSize) {
            device->waitForReadyRead(-1);
        }
        bytesRead = device->read((char*)&packetSize, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = device->read((char*)&packetCounter, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = device->read((char*)&packetTime, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = device->read((char*)&numPackets, (qint64)sizeof(qint32));
        totalBytesRead+=bytesRead;
        bytesRead = device->read((char*)&reportInterval, (qint64)sizeof(qint32));
        totalBytesRead+=bytesRead;
    }

    size_t dataSize = packetSize - headerSize;
    int numSamples = dataSize / sizeof(quint32);

    // Resize the data blob to hold the chunk.
    StreamData* blob = (StreamData*)dataBlob();
    blob->setPacketSize(packetSize);
    blob->setPacketId(packetCounter);
    blob->setTimeStamp(packetTime);
    blob->setTotalPackets(numPackets);
    blob->setReportInterval(reportInterval);
    blob->resize(numSamples);
    quint32* values = blob->ptr();

    // Read the data section of the chunk.
    quint64 minReadSize = 1024;
    quint64 dataRemaining = packetSize-headerSize;
    while (dataRemaining > 0) {
        if (dataRemaining < minReadSize) minReadSize = dataRemaining;
        while (device->bytesAvailable() < (qint64)minReadSize) {
            device->waitForReadyRead(-1);
        }
        bytesRead = device->read((char*)values+totalBytesRead, dataRemaining);
        totalBytesRead+=bytesRead;
        dataRemaining-=bytesRead;
    }
}
