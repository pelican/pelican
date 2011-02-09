#ifndef REALUDPEMULATOR_H
#define REALUDPEMULATOR_H

/**
 * @file RealUdpEmulator.h
 */

#include "pelican/emulator/AbstractUdpEmulator.h"
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

    private:
        double _initialValue;      // Initial value of the packet data.
        unsigned long _packetSize; // Size of the packet.
        unsigned long _interval;   // Interval between packets in microseconds.
        QByteArray _packet;        // Packet data.
        unsigned long _counter;    // Packet counter.
};

} // namespace test
} // namespace pelican
#endif // REALUDPEMULATOR_H
