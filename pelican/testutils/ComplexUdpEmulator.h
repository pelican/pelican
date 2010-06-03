#ifndef COMPLEX_UDP_EMULATOR_H
#define COMPLEX_UDP_EMULATOR_H

#include "pelican/testutils/AbstractUdpEmulator.h"
#include <QtCore/QByteArray>

/**
 * @file RealUdpEmulator.h
 */

namespace pelican {

class ConfigNode;

/**
 * @class ComplexUdpEmulator
 *
 * @brief
 * Fills lofar style UDP packets with complex time subband stream data.
 * 
 * @details
 *
 * \section Configuration:
 *
 * \verbatim
 *      <ComplexUdpEmulator>
 *
 *
 *      </ComplexUdpEmulator>
 * \verbatim
 *
 */

class ComplexUdpEmulator : public AbstractUdpEmulator
{
    public:
        /// Constructs a new UDP packet emulator.
        ComplexUdpEmulator(const ConfigNode& configNode);

        /// Destroys the UDP packet emulator.
        ~ComplexUdpEmulator() {}

        /// Creates a UDP packet.
        void getPacketData(char*& ptr, unsigned long& size);

        /// Returns the interval between packets in microseconds.
        unsigned long interval() { return _interval; }

    private:
        double _initialValue;       ///< The initial value of the packet data.
        unsigned long _packetSize; ///< The size of the packet.
        unsigned long _interval;   ///< The interval between packets in microseconds.
        QByteArray _packet;          ///< The data packet.
        unsigned long _counter;    ///< Packet counter.
};

} // namespace pelican

#endif // COMPLEX_UDP_EMULATOR_H
