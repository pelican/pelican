#include "pelican/emulator/test/RealUdpEmulator.h"
#include "pelican/emulator/EmulatorDriver.h"

#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

using namespace pelican;
using test::RealUdpEmulator;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    // Get command line arguments.
    if (argc != 5) {
        std::cerr << "Usage: realUdpEmulator <port> <host> "
                "<interval, microsec> <packet size, bytes>" << std::endl;
        return 1;
    }
    QString port = QString::fromAscii(argv[1]);
    QString host = QString::fromAscii(argv[2]);
    QString interval = QString::fromAscii(argv[3]);
    QString size = QString::fromAscii(argv[4]);

    // Start the emulator.
    ConfigNode emulatorConfig(
            "<RealUdpEmulator>"
            "    <connection host=\"" + host + "\" port=\"" + port + "\"/>"
            "    <packet size=\"" + size + "\" interval=\"" + interval + "\"/>"
            "</RealUdpEmulator>"
            );
    EmulatorDriver emulator(new RealUdpEmulator(emulatorConfig));

    return app.exec();
}
