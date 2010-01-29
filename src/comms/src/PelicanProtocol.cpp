#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "AcknowledgementRequest.h"
#include "utility/memCheck.h"

namespace pelican {


// class PelicanProtocol 
PelicanProtocol::PelicanProtocol()
    : AbstractProtocol()
{
}

PelicanProtocol::~PelicanProtocol()
{
}

ServerRequest PelicanProtocol::request(QTcpSocket& socket) 
{

    int timeout = 1000;
    ServerRequest::Request_t type = ServerRequest::Error;
    while (socket.bytesAvailable() < (int)sizeof(quint16)) {
        if ( !socket.waitForReadyRead(timeout) ) {
            return ServerRequest(type,  socket.errorString() );
        }
    }

    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_4_0);
    in >> (quint16&)type;

    switch(type)
    {
        case ServerRequest::Acknowledge:
            return AcknowledgementRequest();
            break;
        default:
            return ServerRequest(ServerRequest::Error, "Unknown type Passed");
            break;
    }
}

void PelicanProtocol::send(QByteArray& stream, const AbstractProtocol::StreamData_t& ) 
{
}

void PelicanProtocol::send(QByteArray& stream, const AbstractProtocol::ServiceData_t& ) 
{
}

void PelicanProtocol::send(QByteArray& stream, const QString& msg )
{
    QDataStream out(&stream, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(stream.size() - sizeof(quint16));
}

void PelicanProtocol::sendError(QByteArray& stream, const QString& msg)
{
    QDataStream out(&stream, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(stream.size() - sizeof(quint16));
}

} // namespace pelican
