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

namespace pelican {

// class PelicanServer
PelicanServer::PelicanServer(QObject* parent)
    : QThread(parent)
{
}

PelicanServer::~PelicanServer()
{
    terminate();
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
    _addChunker(chunker,host,port);
    _streamDataTypes.insert( chunker->type() );
}

void PelicanServer::addServiceChunker( AbstractChunker* chunker, const QString& host, quint16 port)
{
    _addChunker(chunker,host,port);
    _serviceDataTypes.insert( chunker->type() );
}


void PelicanServer::_addChunker( AbstractChunker* chunker, const QString& host, quint16 port)
{
    if( chunker ) {
        if( _streamDataTypes.contains(chunker->type()) || _serviceDataTypes.contains(chunker->type()) ) {
            throw( QString("PelicanServer:  input stream \"") + chunker->type() + "\" is already assigned");
        }
        QPair<QString,quint16> pair(host,port);
        if( _chunkerPortMap.contains(pair) ) {
            delete chunker;
            throw QString("Cannot map multiple chunkers to the same host/port ("
                    + host + "/" + QString().setNum(port)) + ")";
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
        boost::shared_ptr<DataReceiver> listener( new DataReceiver(_chunkerPortMap[inputPorts[i]], &dataManager ) );
        chunkers.append(listener);
        listener->listen( inputPorts[i].first, inputPorts[i].second );
    }

    // set up listening servers
    QList<quint16> ports = _protocolPortMap.keys();
    for (int i = 0; i < ports.size(); ++i) {
        boost::shared_ptr<PelicanPortServer> server( new PelicanPortServer(_protocolPortMap[ports[i]], &dataManager ) );
        servers.append(server);
        if ( !server->listen(QHostAddress::Any, ports[i] )) {
            throw QString(QString("Unable to start pelicanServer on port='") + QString().setNum(ports[i])+ QString("'"));
        }
    }
    exec();
}

} // namespace pelican
