#include "pelican/testutils/ComplexUdpEmulator.h"
#include "pelican/utility/ConfigNode.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the ComplexUdpEmulator.
 * This obtains the relevant configuration parameters.
 */
ComplexUdpEmulator::ComplexUdpEmulator(const ConfigNode& configNode)
    : AbstractUdpEmulator(configNode)
{
//    // Initialise members.
//    _counter = 0;
//    _interval = configNode.getOption("packet", "interval", "100000").toULong();
//    _packetSize = configNode.getOption("packet", "size", "8192").toULong();
//    _initialValue = configNode.getOption("packet", "initialValue", "0").toDouble();
//
//    // Check that the packet size is a multiple of 8.
//    if (_packetSize % sizeof(double) != 0) {
//        throw QString("ComplexUdpEmulator: "
//                "Packet size must be a whole number of doubles.");
//    }
//
//    // Check that the packet size is not empty.
//    if (_packetSize == 0) {
//        throw QString("ComplexUdpEmulator: Packet size must be > 0.");
//    }
//
//    // Initialise the packet.
//    _packet.resize(_packetSize);
}

/**
 * @details
 *
 * @param ptr[out]   Pointer to the start of the packet.
 * @param size[out]  Size of the packet.
 */
void ComplexUdpEmulator::getPacketData(char*& ptr, unsigned long& size)
{
//    // Set the output data.
//    ptr = _packet.data();
//    size = _packetSize;
//
//    // Fill the packet.
//    unsigned nDoubles = _packet.size() / sizeof(double);
//    for (unsigned i = 0; i < nDoubles; ++i) {
//        reinterpret_cast<double*>(ptr)[i] = _initialValue + _counter;
//    }
//
//    // Increment counter for next time.
//    _counter++;
}

} // namespace pelican
