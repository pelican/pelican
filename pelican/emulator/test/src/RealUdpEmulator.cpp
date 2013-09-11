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

#include "emulator/test/RealUdpEmulator.h"
#include "utility/ConfigNode.h"

#include <iostream>

namespace pelican {
namespace test {

/**
 * @details Constructs the RealUdpEmulator.
 * This obtains the relevant configuration parameters.
 */
RealUdpEmulator::RealUdpEmulator(const ConfigNode& configNode)
    : AbstractUdpEmulator(configNode)
{
    // Initialise members.
    _counter = 0;
    _interval = configNode.getOption("packet", "interval", "100000").toULong();
    _packetSize = configNode.getOption("packet", "size", "8192").toULong();
    _initialValue = configNode.getOption("packet", "initialValue", "0").toDouble();
    // Default of -1 == run forever.
    _numPackets = configNode.getOption("packet", "number", "-1").toInt();

    // Check that the packet size is a multiple of 8.
    if (_packetSize % sizeof(double) != 0)
        throw QString("RealUdpEmulator: "
                "Packet size must be a whole number of doubles.");

    // Check that the packet size is not empty.
    if (_packetSize == 0)
        throw QString("RealUdpEmulator: Packet size must be > 0.");

    // Initialise the packet.
    _packet.resize(_packetSize);
}

/**
 * @details
 * Creates a packet of real-valued double-precision data.
 * The values increase linearly by 1.0 each time this method is called.
 *
 * @param ptr[out]   Pointer to the start of the packet.
 * @param size[out]  Size of the packet.
 */
void RealUdpEmulator::getPacketData(char*& ptr, unsigned long& size)
{
    // Set the output data.
    ptr = _packet.data();
    size = _packetSize;

    // Fill the packet.
    unsigned nDoubles = _packet.size() / sizeof(double);
    for (unsigned i = 0; i < nDoubles; ++i)
        reinterpret_cast<double*>(ptr)[i] = _initialValue + _counter;

    // Increment counter for next time.
    _counter++;
}

int RealUdpEmulator::nPackets()
{
    return _numPackets;
}

void RealUdpEmulator::emulationFinished()
{
    std::cout << "RealUdpEmulator: emulation Finished" << std::endl;
}


} // namespace test
} // namespace pelican
