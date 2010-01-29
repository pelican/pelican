#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include "PelicanProtocol.h"
#include "data/ServerRequest.h"
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
    ServerRequest::Request_t type = ServerRequest::Error;
    if ( socket.waitForReadyRead(1000) ) {
        QStringList tokens =
            QString(socket.readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if( ! tokens.isEmpty() )
                        type = (ServerRequest::Request_t)tokens.first().toInt() ;
    }
    else {
        return ServerRequest(type, "Timeout waiting for client");
    }
    ServerRequest r(type);
    return r;
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
