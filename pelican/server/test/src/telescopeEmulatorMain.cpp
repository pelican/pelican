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
    if (argc != 5) {
        std::cerr << "Usage: telescopeEmulator <port> <host> "
                "<interval, microsec> <chunk size, bytes>" << std::endl;
        return 1;
    }
    int port = atoi(argv[1]);
    QHostAddress host(QString::fromAscii((argv[2])));
    int interval = atoi(argv[3]);
    int chunkSize = atoi(argv[4]);
    TelescopeEmulator emulator(0, port, host, interval, chunkSize);

    return app.exec();
}
