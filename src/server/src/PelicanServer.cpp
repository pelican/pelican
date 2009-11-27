#include "PelicanServer.h"
#include "Session.h"


namespace pelican {


// class PelicanServer 
PelicanServer::PelicanServer(QObject* parent)
    : QTcpServer(parent)
{
}

PelicanServer::~PelicanServer()
{
}

void PelicanServer::incomingConnection(int socketDescriptor)
{
    Session *thread = new Session(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void PelicanServer::start()
{
    if (!listen()) {
        throw QString("Unable to start pelicanServer");
    }
}

} // namespace pelican
