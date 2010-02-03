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
        throw QString("Cannot map multiple protocols to the same port (" + QString().setNum(port));
    }
    _protocolPortMap[port] = proto;
}

void PelicanServer::addChunker(AbstractChunker* chunker, const QString& host, quint16 port)
{
    if( chunker ) {
        QPair<QString,quint16> pair(host,port);
        if( _chunkerPortMap.contains(pair) ) {
            delete chunker;
            throw QString("Cannot map multiple chunkers to the same host/port (" + host + "/" + QString().setNum(port)) + ")";
        }
        _chunkerPortMap[pair] = chunker;
    }
}

void PelicanServer::run()
{
    QVector<boost::shared_ptr<PelicanPortServer> > servers;
    QVector<boost::shared_ptr<DataReceiver> > chunkers;
    DataManager dataManager;
    QList<QPair<QString,quint16> > inputPorts = _chunkerPortMap.keys();
    // set up listening servers
    QList<quint16> ports = _protocolPortMap.keys();
    for (int i = 0; i < ports.size(); ++i) {
        boost::shared_ptr<PelicanPortServer> server( new PelicanPortServer(_protocolPortMap[ports[i]], &dataManager ) );
        servers.append(server);
        if ( !server->listen(QHostAddress::Any, ports[i] )) {
            throw QString(QString("Unable to start pelicanServer on port='") + QString().setNum(ports[i])+ QString("'"));
        }
    }
    // set up datastream inputs
    for (int i = 0; i < inputPorts.size(); ++i) {
        boost::shared_ptr<DataReceiver> listener( new DataReceiver(_chunkerPortMap[inputPorts[i]], &dataManager ) );
        chunkers.append(listener);
        listener->listen( inputPorts[i].first, inputPorts[i].second );
    }
    exec();
}

} // namespace pelican
