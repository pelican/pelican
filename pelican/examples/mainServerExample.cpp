#include "pelican/server/PelicanServer.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/utility/Config.h"

#include "pelican/examples/ChunkerExample.h"

#include <QtCore/QCoreApplication>
#include <iostream>

using namespace pelican;

/* Example Pelican server main()
 *
 * - Create a Pelican configuration (XML) object.
 * - This creates and starts a server.
 * - Attaches a chunker to process the incoming stream.
 * - Attaches a protocol for communication with a Pelican server client.
 * - Enters a QCoreApplication main event loop.
 *
 * note: Pelican objects throws a number of QString messages on various errors so
 * catching these in a main is generally good practice.
 */

int main(int argc, char ** argv)
{
    // Create a QCoreApplication.
    QCoreApplication app(argc, argv);

    // Create a Pelican configuration object.
    Config config;

    try {
        // Create a Pelican server.
        PelicanServer server(&config);

        // Attach the chunker to server.
        server.addStreamChunker("ChunkerExample");

        // Create a communication protocol object and attach it to the server
        // on port 2000.
        AbstractProtocol * protocol =  new PelicanProtocol;
        server.addProtocol(protocol, 2000);

        // Start the server.
        server.start();

        // When the server is ready enter the QCoreApplication event loop.
        while (!server.isReady()) {}
        return app.exec();
    }

    catch (const QString& err)
    {
        std::cerr << "Error: " << err.toStdString() << endl;
    }
}
