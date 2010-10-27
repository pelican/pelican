#include "pelican/examples/SignalEmulator.h"
#include "pelican/utility/ConfigNode.h"

namespace pelican {

/*
 * Constructs the SignalEmulator.
 * This obtains the relevant configuration parameters.
 */
SignalEmulator::SignalEmulator(const ConfigNode& configNode)
    : AbstractUdpEmulator(configNode)
{
    // Initialise defaults.
    _counter = 0;
    _totalSamples = 0;
    _samples = configNode.getOption("packet", "samples", "256").toULong();
    _interval = configNode.getOption("packet", "interval",
            QString::number(_samples * 10)).toULong();

    // Set the packet size in bytes (+32 for header).
    _packet.resize(_samples * sizeof(float) + 32);

    // Set constant parts of packet header data.
    char* ptr = _packet.data();
    *reinterpret_cast<int*>(ptr + 0) = size;
    *reinterpret_cast<int*>(ptr + 4) = 32;
    *reinterpret_cast<int*>(ptr + 8) = _samples;
    *reinterpret_cast<int*>(ptr + 12) = sizeof(float);
}

/*
 * Creates a packet of UDP signal data containing a sine wave, setting the
 * pointer to the start of the packet, and the size of the packet.
 */
void SignalEmulator::getPacketData(char*& ptr, unsigned long& size)
{
    // Set pointer to the output data.
    ptr = _packet.data();
    size = _packet.size();

    // Set the packet header.
    *reinterpret_cast<long int*>(ptr + 16) = _counter;

    // Fill the packet data.
    const char* data = ptr + 32; // Add offset for header.
    for (unsigned i = 0; i < _samples; ++i)
        reinterpret_cast<float*>(data)[i] = _counter; // FIXME write a sine-wave.

    // Increment counters for next time.
    _counter++;
    _totalSamples += _samples;
}

} // namespace pelican
