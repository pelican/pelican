#include "pelican/server/PelicanServer.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/utility/Config.h"

#include "tutorial/SignalChunker.h"

#include <QtCore/QCoreApplication>
#include <iostream>

using namespace pelican;

int main(int argc, char ** argv)
{
    // 1. Create a QCoreApplication.
    QCoreApplication app(argc, argv);

    // 2. Create a Pelican configuration object (this assumes that a Pelican
    // configuration XML file is supplied as the first command line argument)
    if (argc != 2) {
        std::cerr << "Please supply an XML config file." << std::endl;
        return 0;
    }
    QString configFile(argv[1]);
    Config config(configFile);

    try {
        // 3. Create a Pelican server.
        PelicanServer server(&config);

        // 4. Attach the chunker to server.
        server.addStreamChunker("SignalChunker");

        // 5. Create a communication protocol object and attach it to the server
        // on port 2000.
        AbstractProtocol *protocol =  new PelicanProtocol;
        server.addProtocol(protocol, 2000);

        // Start the server.
        server.start();

        // 6. When the server is ready enter the QCoreApplication event loop.
        while (!server.isReady()) {}
        return app.exec();
    }

    // 7. Catch any error messages from Pelican.
    catch (const QString& err)
    {
        std::cerr << "Error: " << err.toStdString() << std::endl;
        return 1;
    }
}
