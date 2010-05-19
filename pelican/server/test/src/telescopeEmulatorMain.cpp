#include "pelican/testutils/EmulatorDriver.h"
#include "pelican/testutils/RealUdpEmulator.h"
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
    QString host = QString::fromAscii(argv[2]);
    int interval = atoi(argv[3]);
    int chunkSize = atoi(argv[4]);

    // Start the telescope.
    ConfigNode emulatorConfig(""
            "<RealUdpEmulator>"
            "    <connection host=\"" + host + "\" port=\"" + QString::number(port) + "\"/>"
            "    <packet size=\"" + QString::number(chunkSize) + "\" interval=\"" + QString::number(interval) + "\"/>"
            "</RealUdpEmulator>"
            );
    EmulatorDriver emulator(new RealUdpEmulator(emulatorConfig));

    return app.exec();
}
