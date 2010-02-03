#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "AcknowledgementRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "data/DataRequirements.h"
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

boost::shared_ptr<ServerRequest> PelicanProtocol::request(QTcpSocket& socket) 
{

    int timeout = 1000;
    ServerRequest::Request_t type = ServerRequest::Error;
    while (socket.bytesAvailable() < (int)sizeof(quint16)) {
        if ( !socket.waitForReadyRead(timeout) ) {
            return boost::shared_ptr<ServerRequest>(new ServerRequest(type,  socket.errorString() ));
        }
    }

    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_4_0);
    in >> (quint16&)type;

    switch(type)
    {
        case ServerRequest::Acknowledge:
            return boost::shared_ptr<AcknowledgementRequest>( new AcknowledgementRequest() );
            break;
        case ServerRequest::ServiceData:
            {
                boost::shared_ptr<ServiceDataRequest> s(new ServiceDataRequest);
                quint16 num;
                in >> (quint16&)num;
                for(int i=0; i < num; ++i )
                {
                    QString type;
                    QString version;
                    in >> type >> version;
                    s->request(type,version);
                }
                return s;
            }
            break;
        case ServerRequest::StreamData:
            {
                boost::shared_ptr<StreamDataRequest> s(new StreamDataRequest);
                quint16 num;
                in >> num;
                for(int i=0; i < num; ++i )
                {
                    QSet<QString> serviceData;
                    QSet<QString> streamData;
                    in >> serviceData;
                    in >> streamData;
                    DataRequirements dr;
                    dr.setServiceData(serviceData);
                    dr.setStreamData(streamData);
                    s->addDataOption(dr);
                }
                return s;
            }
            break;
        default:
            break;
    }
    return boost::shared_ptr<ServerRequest>(new ServerRequest(ServerRequest::Error, "Unknown type Passed"));
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
