#include "pelican/server/PelicanServer.h"

#include "pelican/comms/AbstractProtocol.h"
#include "pelican/server/AbstractChunker.h"
#include "pelican/server/ChunkerManager.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/server/PelicanPortServer.h"
#include "pelican/utility/Config.h"

#include <boost/shared_ptr.hpp>

#include <QtCore/QtGlobal>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDebug>

#include <iostream>

namespace pelican {

/**
 * @details
 * Creates a new Pelican server, which in turn creates a chunker manager.
 */
PelicanServer::PelicanServer(const Config* config, QObject* parent) :
    QThread(parent), _verboseLevel(0)
{
    _config = config;
    _ready = false;

    // Create the chunker manager.
    _chunkerManager = new ChunkerManager(config);
}

/**
 * @details
 * Destroys the Pelican server.
 */
PelicanServer::~PelicanServer()
{
    // Delete the chunker manager (must be destroyed before data manager).
    delete _chunkerManager;

    // Wait for the thread to finish.
    do quit(); while (!wait(10));

    // Delete the protocols.
    foreach (AbstractProtocol* protocol, _protocolPortMap)
        delete protocol;
}

/**
 * @details
 * Adds the given \p protocol to the given \p port.
 * The class takes ownership of \p protocol.
 *
 * @param proto A pointer to the allocated protocol.
 * @param port  The incoming port number to use.
 */
void PelicanServer::addProtocol(AbstractProtocol* protocol, quint16 port)
{
    if ( _protocolPortMap.contains(port) ) {
        delete protocol;
        throw QString("Cannot map multiple protocols to port %1").arg(port);
    }
    _protocolPortMap[port] = protocol;
}

/**
 * @details
 * Adds a stream chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
void PelicanServer::addStreamChunker(QString type, QString name)
{
    _chunkerManager->addStreamChunker(type, name);
}

void PelicanServer::verbose(const QString& msg, int level)
{
     if( level < _verboseLevel )
        std::cout << msg.toStdString() << std::endl;
}

/**
 * @details
 * Adds a service chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
void PelicanServer::addServiceChunker(QString type, QString name)
{
    _chunkerManager->addServiceChunker(type, name);
}

/**
 * @details
 * Runs the server thread starting its event loop.
 *
 * Sets up the data manager which handles stream and service data buffers
 * which are set up on request of the chunkers.
 */
void PelicanServer::run()
{
    try {
        QVector<boost::shared_ptr<PelicanPortServer> > servers;

        // Set up the data manager.
        DataManager dataManager(_config);
        dataManager.setVerbosity(_verboseLevel);
        _chunkerManager->init(dataManager);

        // Set up listening servers.
        QList<quint16> ports = _protocolPortMap.keys();
        for (int i = 0; i < ports.size(); ++i) {
            boost::shared_ptr<PelicanPortServer> server(
                    new PelicanPortServer(_protocolPortMap[ports[i]], &dataManager) );
            server->setVerbosity(_verboseLevel);
            servers.append(server);
            if ( !server->listen(QHostAddress::Any, ports[i]) )
                throw QString("Cannot run PelicanServer on port %1").arg(ports[i]);
        }

        // Set ready flag.
        _mutex.lock();
        _ready = true;
        _mutex.unlock();

        // Enter the server's event loop.
        exec();
    }
    catch( QString& e ) {
        std::cerr << "PelicanServer caught an error: " << e.toStdString() << std::endl;
        verbose(QString("Shutting down server thread: ") + e);
        exit(1);
    }
}

void PelicanServer::setVerbosity(int level) {
     _verboseLevel = level;
}
} // namespace pelican
