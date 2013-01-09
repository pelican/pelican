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

#include "StreamDataEmulator.hpp"
#include <pelican/utility/ConfigNode.h>
#include <cmath>
#include <iostream>

using namespace std;

StreamDataEmulator::StreamDataEmulator(const pelican::ConfigNode& config)
: AbstractUdpEmulator(config), packetCounter_(0), totalSamples_(0)
{
    // Load options (with defaults).
    numSamples_     = config.getOption("packet", "samples", "10").toULong();
    packetInterval_ = config.getOption("packet", "interval", "10000").toULong();
    dataPeriod_     = config.getOption("data", "period", "100").toULong();
    nPackets_       = config.getOption("data", "numPackets", "-1").toInt();

    // Set the packet size in bytes (+32 for the header).
    packet_.resize(numSamples_ * sizeof(float) + 32);

    // Set constant parts of the packet header data
    char* ptr = packet_.data();
    *reinterpret_cast<int*>(ptr + 0)  = packet_.size();
    *reinterpret_cast<int*>(ptr + 4)  = 32;
    *reinterpret_cast<int*>(ptr + 8)  = numSamples_;
    *reinterpret_cast<int*>(ptr + 12) = sizeof(float);
}

StreamDataEmulator::~StreamDataEmulator()
{
}


void StreamDataEmulator::getPacketData(char*& ptr, unsigned long& size)
{
    // Set pointer to the output data.
    ptr  = packet_.data();
    size = packet_.size();

    // Set variable part of the packet header.
    *reinterpret_cast<long int*>(ptr + 16) = packetCounter_;

    // Fill the packet data.
    char* data = ptr + 32;
    for (quint32 i = 0; i < numSamples_; ++i)
    {
        float value = (totalSamples_ + i) % dataPeriod_;
        reinterpret_cast<float*>(data)[i] = value;
    }

    qDebug() << "Stream: [" << packetCounter_ << "] Bytes sent = " << size
             << " (total = "
             << size * packetCounter_ << ")";

    ++packetCounter_;
    totalSamples_ += numSamples_;
}

void StreamDataEmulator::emulationFinished()
{
    cout << "StreamDataEmulator::emulationFinished()" << endl;
    cout << " -- Packets sent = " << packetCounter_ << endl;
    cout << " -- Packet size  = " << packet_.size() << endl;
    cout << " -- Total bytes sent = " << packetCounter_ * packet_.size() <<  endl;
}

int StreamDataEmulator::nPackets()
{
    return nPackets_;
}

unsigned long StreamDataEmulator::interval()
{
    return packetInterval_;
}
