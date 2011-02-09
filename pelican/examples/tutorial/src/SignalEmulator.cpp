#include "tutorial/SignalEmulator.h"
#include "pelican/utility/ConfigNode.h"
#include <cmath>

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
            QString::number(_samples * 10)).toULong(); // Interval in micro-sec.
    _period = configNode.getOption("signal", "period", "20").toULong();
    _omega = (2.0 * 3.14159265) / _period; // Angular frequency.

    // Set the packet size in bytes (+32 for header).
    _packet.resize(_samples * sizeof(float) + 32);

    // Set constant parts of packet header data.
    char* ptr = _packet.data();
    *reinterpret_cast<int*>(ptr + 0) = _packet.size(); // Total bytes in packet.
    *reinterpret_cast<int*>(ptr + 4) = 32; // Size of the header in bytes.
    *reinterpret_cast<int*>(ptr + 8) = _samples; // Samples in the packet.
    *reinterpret_cast<int*>(ptr + 12) = sizeof(float); // Bytes per sample.
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
    char* data = ptr + 32; // Add offset for header.
    for (unsigned i = 0; i < _samples; ++i) {
        float value = sin(((_totalSamples + i) % _period) * _omega);
        reinterpret_cast<float*>(data)[i] = value;
    }

    // Increment counters for next time.
    _counter++;
    _totalSamples += _samples;
}

} // namespace pelican
