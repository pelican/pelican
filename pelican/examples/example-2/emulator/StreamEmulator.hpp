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

#ifndef STREAMEMULATOR_HPP_
#define STREAMEMULATOR_HPP_

#include <pelican/emulator/AbstractEmulator.h>
#include <pelican/utility/ConfigNode.h>

#include <QtCore/QbyteArray>
#include <QtCore/QTime>
#include <QtNetwork/QHostAddress>

class StreamEmulator : public pelican::AbstractEmulator
{
public:
    StreamEmulator(const pelican::ConfigNode& config);
    ~StreamEmulator();

    void getPacketData(char*& data, unsigned long& bytes);
    unsigned long interval();
    int nPackets();

private:
    void emulationFinished();
    QIODevice* createDevice();

    QHostAddress host_;
    quint16 port_;
    quint32 numPackets_;
    size_t  numSamples_;
    size_t  packetSize_;
    quint32 packetInterval_;
    quint32 packetCounter_;

    QByteArray packet_; // Note this is the serialised packet.

    QTime timer_;
};


#endif /* STREAMEMULATOR_HPP_ */
