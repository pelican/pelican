#include "TelescopeEmulator.h"
#include <iostream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QString>

using namespace pelican;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    // Get command line arguments.
    if (argc != 3) {
        std::cerr << "Usage: telescopeEmulator <port> <host>" << std::endl;
        return 1;
    }
    int port = atoi(argv[1]);
    QHostAddress host(QString::fromAscii((argv[2])));
    TelescopeEmulator emulator(0, port, host);

    return app.exec();
}
