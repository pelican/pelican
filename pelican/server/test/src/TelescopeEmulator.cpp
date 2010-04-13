#include "pelican/server/test/TelescopeEmulator.h"
#include <QUdpSocket>

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new telescope emulator, which starts running in its
 * own thread. The data will be sent down the specified UDP \p port, and
 * will start with the given \p initialValue.
 */
TelescopeEmulator::TelescopeEmulator(const qint16 port, const double initialValue)
{
    _abort = false;
    _port = port;
    _initialValue = initialValue;
    start();
}

/**
 * @details
 * Destroys the telescope emulator, aborting the thread that it runs in.
 */
TelescopeEmulator::~TelescopeEmulator()
{
    _abort = true;
    wait();
}

/**
 * @details
 * Runs the telescope emulator in its own thread.
 * A 512-byte UDP datagram is sent via a network socket every millisecond
 * to the local host.
 */
void TelescopeEmulator::run()
{
    QUdpSocket socket;

    unsigned counter = 0;
    while (!_abort) {
        double value = double(counter) + _initialValue;
        std::vector<double> data(64, value);
        socket.writeDatagram(reinterpret_cast<char*>(&data[0]), 512,
                QHostAddress::LocalHost, _port);
        msleep(1);
        ++counter;
    }
}

} // namespace pelican
