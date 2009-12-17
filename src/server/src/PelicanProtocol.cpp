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

void PelicanProtocol::send(QDataStream& stream, const QList<DataBlob>& ) 
{
}

void PelicanProtocol::send(QDataStream& stream, const QString& msg )
{
    stream << msg;
}

void PelicanProtocol::sendError(QDataStream& stream, const QString&)
{
}

} // namespace pelican
