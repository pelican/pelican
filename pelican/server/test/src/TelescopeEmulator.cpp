#include "pelican/server/test/TelescopeEmulator.h"
#include <QUdpSocket>
#include <QTimer>

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new telescope emulator, which starts running in its
 * own thread using the given \p initialValue.
 *
 * The data will be sent to the specified UDP \p port on the given \p host,
 * with the given \p interval in microseconds.
 *
 * The \p chunkSize is given in bytes.
 */
TelescopeEmulator::TelescopeEmulator(const double initialValue,
        const qint16 port, const QHostAddress host, const int interval,
        const int chunkSize) : QThread()
{
    _abort = false;
    _initialValue = initialValue;
    _host = host;
    _port = port;
    _interval = interval;
    _chunkSize = chunkSize;
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
 * Runs the telescope emulator in its own thread, sending the data down a
 * local UDP socket.
 */
void TelescopeEmulator::run()
{
    QUdpSocket socket;

    unsigned counter = 0;
    unsigned nDoubles = _chunkSize / sizeof(double);
    if (_chunkSize % sizeof(double) != 0) nDoubles++;
    std::vector<double> data(nDoubles);

    while (!_abort) {
        // Set the array.
        for (unsigned i = 0; i < nDoubles; ++i)
            data[i] = counter + _initialValue;

        // Write the data.
        socket.writeDatagram(reinterpret_cast<char*>(&data[0]),
                _chunkSize, _host, _port);

        // Sleep.
        usleep(_interval);
        ++counter;
    }
}

} // namespace pelican
