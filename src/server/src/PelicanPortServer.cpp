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
PelicanPortServer::PelicanPortServer(AbstractProtocol* proto, QObject* parent)
    : QTcpServer(parent), _proto(proto)
{
}

PelicanPortServer::~PelicanPortServer()
{
}

void PelicanPortServer::incomingConnection(int socketDescriptor)
{
    Session *thread = new Session(socketDescriptor, _proto, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

} // namespace pelican
