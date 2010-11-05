#ifndef SIGNALEMULATOR_H
#define SIGNALEMULATOR_H

#include "pelican/emulator/AbstractUdpEmulator.h"
#include <QtCore/QByteArray>

namespace pelican {

class ConfigNode;

/*
 * This emulator outputs simple packets of real-valued, floating-point,
 * time-series data using a UDP socket. A sine wave is put into the time
 * stream.
 * It should be constructed with a configuration node that contains
 * the number of samples in the packet, the interval between packets in
 * microseconds, and the connection settings.
 *
 * The default values are:
 *
 * <SignalEmulator>
 *     <packet samples="256" interval="2560" />
 *     <signal period="20" />
 *     <connection host="127.0.0.1" port="2001" />
 * </SignalEmulator>
 */
class SignalEmulator : public AbstractUdpEmulator
{
    public:
        // Constructs a new UDP packet emulator.
        SignalEmulator(const ConfigNode& configNode);

        // Destroys the UDP packet emulator.
        ~SignalEmulator() {}

        // Creates a UDP packet.
        void getPacketData(char*& ptr, unsigned long& size);

        // Returns the interval between packets in microseconds.
        unsigned long interval() {return _interval;}

    private:
        unsigned long _counter;  // Packet counter.
        unsigned long long _totalSamples; // The total number of samples sent.
        unsigned long _samples; // The number of samples in the packet.
        unsigned long _interval; // The interval between packets in microsec.
        unsigned long _period; // The number of samples in one period.
        float _omega; // The angular frequency of the signal (from _period).
        QByteArray _packet; // The data packet.
};

} // namespace pelican

#endif // SIGNALEMULATOR_H
