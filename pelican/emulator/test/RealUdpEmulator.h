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

#ifndef REALUDPEMULATOR_H
#define REALUDPEMULATOR_H

/**
 * @file RealUdpEmulator.h
 */

#include "emulator/AbstractUdpEmulator.h"
#include <QtCore/QByteArray>

namespace pelican {

class ConfigNode;

namespace test {

/**
 * @ingroup t_emulator
 *
 * @class RealUdpEmulator
 *
 * @brief A real-valued data packet emulator that uses a UDP socket.
 *
 * @details
 * This emulator outputs simple packets of real-valued double-precision data
 * using a UDP socket.
 * It should be constructed with a configuration node that contains
 * the packet size in bytes, the interval between packets in microseconds,
 * and the initial data value.
 *
 * The default values are:
 *
 * @verbatim
 * <packet size="8192" interval="100000" initialValue="0" />
 * @endverbatim
 */

class RealUdpEmulator : public AbstractUdpEmulator
{
    public:
        /// Constructs a new UDP packet emulator.
        RealUdpEmulator(const ConfigNode& configNode);

        /// Destroys the UDP packet emulator.
        ~RealUdpEmulator() {}

        /// Creates a UDP packet.
        void getPacketData(char*& ptr, unsigned long& size);

        /// Returns the interval between packets in microseconds.
        unsigned long interval() {return _interval;}

        int nPackets();

        void emulationFinished();

    private:
        double _initialValue;      // Initial value of the packet data.
        unsigned long _packetSize; // Size of the packet.
        unsigned long _interval;   // Interval between packets in microseconds.
        QByteArray _packet;        // Packet data.
        unsigned long _counter;    // Packet counter.
        int _numPackets;           // Number of packets to send.
};

} // namespace test
} // namespace pelican
#endif // REALUDPEMULATOR_H
