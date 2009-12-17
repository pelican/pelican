#include <QTcpSocket>
#include "PelicanPortServer.h"
#include "Session.h"
#include "AbstractProtocol.h"
#include "utility/memCheck.h"
#ifdef new
#undef new
#endif
#ifdef delete
#undef delete
#endif

namespace pelican {

// class PelicanPortServer 
PelicanPortServer::PelicanPortServer(AbstractProtocol* proto, DataManager* data, QObject* parent)
    : QTcpServer(parent), _proto(proto), _data(data)
{
}

PelicanPortServer::~PelicanPortServer()
{
}

void PelicanPortServer::incomingConnection(int socketDescriptor)
{
    Session *thread = new Session(socketDescriptor, _proto, _data, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

} // namespace pelican
