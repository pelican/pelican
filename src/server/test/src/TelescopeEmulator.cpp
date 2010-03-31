#include "server/test/TelescopeEmulator.h"
#include <QUdpSocket>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


TelescopeEmulator::TelescopeEmulator()
{
    _abort = false;
    start();
}

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
//    socket.bind(QHostAddress::LocalHost, 2002);

    unsigned counter = 0;
    while (!_abort) {
//         std::cout << "TelescopeEmulator::run()" << std::endl;
        std::vector<double> data(64, double(counter));
        socket.writeDatagram(reinterpret_cast<char*>(&data[0]), 512,
                QHostAddress::LocalHost, 2002);
        msleep(1);
        ++counter;
    }
}


} // namespace pelican
