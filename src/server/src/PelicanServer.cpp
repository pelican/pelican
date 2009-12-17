#include <boost/shared_ptr.hpp>
#include <QtGlobal>
#include <QTcpSocket>
#include "PelicanServer.h"
#include "AbstractProtocol.h"
#include "DataManager.h"
#include "PelicanProtocol.h"
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

void PelicanServer::run()
{
    QVector<boost::shared_ptr<PelicanPortServer> > servers;
    QList<quint16> ports = _protocolPortMap.keys();
    DataManager dataManager;
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
