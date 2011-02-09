#ifndef EMULATOREXAMPLE_H
#define EMULATOREXAMPLE_H

#include "pelican/emulator/AbstractUdpEmulator.h"
#include <QtCore/QByteArray>

/**
 * @file EmulatorExample.h
 */

namespace pelican {

class ConfigNode;

/**
 * @ingroup c_examples
 *
 * @class EmulatorExample
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
 * @verbatim <packet size="8192" interval="100000" initialValue="0" /> @endverbatim
 */

/*
 * This emulator outputs simple packets of real-valued double-precision data
 * using a UDP socket.
 * It should be constructed with a configuration node that contains
 * the packet size in bytes, the interval between packets in microseconds,
 * and the initial data value.
 *
 * The default values are:
 *
 * <packet size="8192" interval="100000" initialValue="0" />
 */
class EmulatorExample : public AbstractUdpEmulator
{
    public:
        // Constructs a new UDP packet emulator.
        EmulatorExample(const ConfigNode& configNode);

        // Destroys the UDP packet emulator.
        ~EmulatorExample() {}

        // Creates a UDP packet.
        void getPacketData(char*& ptr, unsigned long& size);

        // Returns the interval between packets in microseconds.
        unsigned long interval() {return _interval;}

    private:
        double _initialValue;      // The initial value of the packet data.
        unsigned long _packetSize; // The size of the packet.
        unsigned long _interval;   // The interval between packets in microseconds.
        QByteArray _packet;        // The data packet.
        unsigned long _counter;    // Packet counter.
};

} // namespace pelican

#endif // EMULATOREXAMPLE_H
