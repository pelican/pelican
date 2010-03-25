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
#include "utility/memCheck.h"
#include <iostream>

namespace pelican {

// class PelicanServer
PelicanServer::PelicanServer(QObject* parent)
    : QThread(parent)
{
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


void PelicanServer::addStreamChunker( AbstractChunker* chunker, const QString& host, quint16 port)
{
    _addChunker(chunker);
    _streamDataTypes.insert( chunker->type() );
}

void PelicanServer::addServiceChunker( AbstractChunker* chunker, const QString& host, quint16 port)
{
    _addChunker(chunker);
    _serviceDataTypes.insert( chunker->type() );
}


void PelicanServer::_addChunker( AbstractChunker* chunker)
{
    if( chunker ) {
        if( _streamDataTypes.contains(chunker->type()) || _serviceDataTypes.contains(chunker->type()) ) {
            throw( QString("PelicanServer:  input stream \"") + chunker->type() + "\" is already assigned");
        }
        QPair<QString,quint16> pair(chunker->host(), chunker->port());
        if (!chunker->host().isEmpty()) {
            if( _chunkerPortMap.contains(pair) ) {
                delete chunker;
                throw QString("Cannot map multiple chunkers to the same host/port ("
                        + chunker->host() + "/" + QString().setNum(chunker->port())) + ")";
            }
        }
        _chunkerPortMap[pair] = chunker;
    }
}

void PelicanServer::run()
{
    QVector<boost::shared_ptr<PelicanPortServer> > servers;
    QVector<boost::shared_ptr<DataReceiver> > chunkers;

    // set up the data manager
    DataManager dataManager;
    foreach( QString type, _streamDataTypes )
    {
        dataManager.getStreamBuffer(type); // creates the buffer
    }
    foreach( QString type, _serviceDataTypes )
    {
        dataManager.getServiceBuffer(type); // creates the buffer
    }

    QList<QPair<QString,quint16> > inputPorts = _chunkerPortMap.keys();

    // set up datastream inputs
    for (int i = 0; i < inputPorts.size(); ++i) {
        AbstractChunker* chunker = _chunkerPortMap[inputPorts[i]];
        chunker->setDataManager(&dataManager); // Added this.
        boost::shared_ptr<DataReceiver> listener( new DataReceiver(chunker, &dataManager ) );
        chunkers.append(listener);
        listener->listen();
    }

    // set up listening servers
    QList<quint16> ports = _protocolPortMap.keys();
    for (int i = 0; i < ports.size(); ++i) {
        boost::shared_ptr<PelicanPortServer> server( new PelicanPortServer(_protocolPortMap[ports[i]], &dataManager ) );
        servers.append(server);
        if ( !server->listen(QHostAddress::Any, ports[i] )) {
            throw QString(QString("Unable to start PelicanServer on port='") + QString().setNum(ports[i])+ QString("'"));
        }
    }
    _mutex.lock();
    _ready = true;
    _mutex.unlock();
    exec();
}

} // namespace pelican
