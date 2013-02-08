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

#include "ServiceDataEmulator.hpp"
#include <pelican/utility/ConfigNode.h>
#include <QtCore/QByteArray>
#include <cmath>
#include <iostream>

using namespace std;

ServiceDataEmulator::ServiceDataEmulator(const pelican::ConfigNode& config)
: AbstractUdpEmulator(config), packetCounter_(0)
{
    // Load options (with defaults).
    packetInterval_ = config.getOption("packet", "interval", "10000").toULong();
    numSamples_ = config.getOption("packet", "samples", "200").toInt();
    numPackets_ = config.getOption("data", "numPackets", "-1").toInt();
    dataPeriod_ = config.getOption("data", "period", "5").toInt();

    // Set the packet size in bytes (+32 for the header).
    packet_.resize(numSamples_ * sizeof(float));
}


ServiceDataEmulator::~ServiceDataEmulator()
{
}


void ServiceDataEmulator::getPacketData(char*& data, unsigned long& size)
{
    // Set pointer to the output data.
    data = packet_.data();
    size = packet_.size();

    // Fill the packet data.
    float value = 1 + (packetCounter_ % dataPeriod_);
    for (int i = 0; i < numSamples_; ++i)
        reinterpret_cast<float*>(data)[i] = value + (0.1/(float)numSamples_);

    cout << "ServiceData, bytes sent = " << size << endl;

    ++packetCounter_;
}

void ServiceDataEmulator::emulationFinished()
{
    cout << "ServiceDataEmulator::emulationFinished(). " << endl;
    cout << " -- Packets sent = " << packetCounter_ << endl;
    cout << " -- Packet size  = " << packet_.size() << endl;
    cout << " -- Total bytes sent = " << packetCounter_ * packet_.size() <<  endl;
}

int ServiceDataEmulator::nPackets()
{
    return numPackets_;
}

unsigned long ServiceDataEmulator::interval()
{
    return packetInterval_;
}
