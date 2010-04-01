#include <boost/shared_ptr.hpp>
#include <QtGlobal>
#include <QTcpSocket>
#include "PelicanServer.h"
#include "comms/AbstractProtocol.h"
#include "AbstractChunker.h"
#include "DataManager.h"
#include "DataReceiver.h"
#include "comms/PelicanProtocol.h"
#include "PelicanPortServer.h"
#include <iostream>
#include "utility/Config.h"

#include "utility/memCheck.h"

namespace pelican {


// class PelicanServer
PelicanServer::PelicanServer(const Config* config, QObject* parent)
    : QThread(parent)
{
    _config = config;
    _ready = false;
}


PelicanServer::~PelicanServer()
{
    if (isRunning()) while (!isFinished()) quit();
    wait();
    foreach(AbstractProtocol* p, _protocolPortMap)
        delete p;
}


void PelicanServer::addProtocol(AbstractProtocol* proto, quint16 port)
{
    if( _protocolPortMap.contains(port) ) {
        delete proto;
        throw QString("Cannot map multiple protocols to the same port ("
                + QString().setNum(port)) + ")";
    }
    _protocolPortMap[port] = proto;
}


void PelicanServer::addStreamChunker( AbstractChunker* chunker)
{
    _chunkerManager.addStreamChunker(chunker);
}


void PelicanServer::addServiceChunker( AbstractChunker* chunker)
{
    _chunkerManager.addServiceChunker(chunker);
}


/**
 * @details
 * Runs the server thread starting its event loop.
 * 
 * Sets up the data manager which handles stream and service data buffers
 * which are set up on request of the chunkers.
 * 
 */
void PelicanServer::run()
{
    QVector<boost::shared_ptr<PelicanPortServer> > servers;

    // Set up the data manager.
    DataManager dataManager(_config);
    _chunkerManager.init(dataManager);

    // Set up listening servers.
    QList<quint16> ports = _protocolPortMap.keys();
    for (int i = 0; i < ports.size(); ++i) {
        boost::shared_ptr<PelicanPortServer> server(
            new PelicanPortServer(_protocolPortMap[ports[i]], &dataManager )
        );
        servers.append(server);
        if ( !server->listen(QHostAddress::Any, ports[i] )) {
            throw QString(QString("Unable to start PelicanServer on port='") +
                            QString().setNum(ports[i])+ QString("'"));
        }
    }
    _mutex.lock();
    _ready = true;
    _mutex.unlock();
    exec();
}

} // namespace pelican
