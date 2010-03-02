#include <QTcpSocket>
#include <QByteArray>
#include <QStringList>
#include <QString>
#include <QMapIterator>
#include "Data.h"
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "ServerResponse.h"
#include "AcknowledgementRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "StreamData.h"
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

void PelicanProtocol::send(QIODevice& stream, const AbstractProtocol::StreamData_t& data ) 
{
    // construct the stream data header
    // first integer is the number of Stream Data sets
    // for each Stream Data set there is a name tag, version tag, and size integer
    // following integer is the number of Service Data sets associated with the stream data
    // for each Service Data set there is a name tag and version tag
    // finally the binary data for the Stream itself is sent
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << ServerResponse::StreamData;
    out << data.size();
    QMapIterator<QString, StreamData*> i(data);
    // stream header info
    while (i.hasNext()) {
        i.next();
        out << i.key() << (i.value())->id() << (quint64)(i.value())->size();
        // service data info
        out << (quint64) i.value()->associateData().size();
        foreach(const Data* dat, i.value()->associateData())
        {
            out << dat->name() << dat->id();
        }
    }
    stream.write(array);
    // actual stream data
    i.toFront();
    while (i.hasNext())
    {
        i.next();
        stream.write( (const char*)i.value()->operator*(), i.value()->size() );
    }
}

void PelicanProtocol::send(QIODevice& stream, const AbstractProtocol::ServiceData_t& data ) 
{
    // construct the service data header
    // first integer is the number of Service Data sets
    // for each set there is a name tag, a version tag and size of data
    // followed by the binary data itself
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    // header data
    out.setVersion(QDataStream::Qt_4_0);
    out << ServerResponse::ServiceData;
    out << data.size();

    QMapIterator<QString, Data*> i(data);
    while (i.hasNext()) {
        i.next();
        out << i.key() << i.value()->id() << (quint64)i.value()->size();
    }
    stream.write(array);

    // binary data
    i.toFront();
    while (i.hasNext())
    {
        i.next();
        stream.write( (const char*)i.value()->operator*(), (quint64)i.value()->size() );
    }
}

void PelicanProtocol::send(QIODevice& stream, const QString& msg )
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(array.size() - sizeof(quint16));
    stream.write(array);
}

void PelicanProtocol::sendError(QIODevice& stream, const QString& msg)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(stream.size() - sizeof(quint16));
    stream.write(array);
}

} // namespace pelican
