#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include "PelicanProtocol.h"
#include "utility/ServerRequest.h"
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
    if ( socket.waitForReadyRead(1000) ) {
        QStringList tokens =
            QString(socket.readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        ServerRequest r(tokens);
        return r;
    }
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
