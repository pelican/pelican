#include "pelican/server/PelicanPortServer.h"
#include "pelican/server/Session.h"
#include "pelican/comms/AbstractProtocol.h"

#include <QtNetwork/QTcpSocket>

#include "pelican/utility/memCheck.h"

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
