#include "server/test/TelescopeEmulator.h"
#include <QUdpSocket>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
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
*/
TelescopeEmulator::~TelescopeEmulator()
{
    _abort = true;
    wait();
}


/**
 * @details
 */
void TelescopeEmulator::run()
{
    QUdpSocket socket;

    unsigned counter = 0;
    while (!_abort) {
//         std::cout << "TelescopeEmulator::run()" << std::endl;
        double value = double(counter) + _initialValue;
        std::vector<double> data(64, value);
        socket.writeDatagram(reinterpret_cast<char*>(&data[0]), 512,
                QHostAddress::LocalHost, _port);
        msleep(1);
        ++counter;
    }
}


} // namespace pelican
