#include "pelican/comms/Data.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/comms/ServerResponse.h"
#include "pelican/comms/AcknowledgementRequest.h"
#include "pelican/comms/ServiceDataRequest.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/StreamData.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"

#include <QtNetwork/QTcpSocket>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QMapIterator>

#include "pelican/utility/memCheck.h"

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
    ServerRequest::Request type = ServerRequest::Error;
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
                    QString dtype;
                    QString version;
                    in >> dtype >> version;
                    s->request(dtype,version);
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
                    dr.addServiceData(serviceData);
                    dr.addStreamData(streamData);
                    s->addDataOption(dr);
                }
                return s;
            }
            break;
        default:
            break;
    }
    return boost::shared_ptr<ServerRequest>(new ServerRequest(ServerRequest::Error, "PelicanProtocol: Unknown type passed"));
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
    out << (quint16)ServerResponse::StreamData;
    out << (quint16)data.size();
    QListIterator<StreamData*> i(data);
    // stream header info
    while (i.hasNext()) {
        StreamData* sd = i.next();
        out << sd->name() << sd->id() << (quint64)(sd->size());
        // service data info
        out << (quint16) sd->associateData().size();
        foreach(const boost::shared_ptr<Data>& dat, sd->associateData())
        {
            out << dat->name() << dat->id() << (quint64)(dat->size());
        }
    }
    stream.write(array);
    // actual stream data
    i.toFront();
    while (i.hasNext())
    {
        StreamData* sd = i.next();
        stream.write( (const char*)sd->operator*(), sd->size() );
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
    out << (quint16)ServerResponse::ServiceData;
    out << (quint16)data.size();

    QListIterator<Data*> i(data);
    while (i.hasNext()) {
        Data* d = i.next();
        out << d->name() << d->id() << (quint64)d->size();
    }
    stream.write(array);

    // binary data
    i.toFront();
    while (i.hasNext())
    {
        Data* d = i.next();
        stream.write( (const char*)d->operator*(), (quint64)d->size() );
    }
}

void PelicanProtocol::send(QIODevice& device, const QString& name, const DataBlob& data )
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)ServerResponse::Blob;
    out << data.type();
    out << name;
    out << data.serialisedBytes();
    if (device.write(array) < 0)
        throw QString("PelicanProtocol::send: Unable to write.");
    data.serialise(device);
}

void PelicanProtocol::send(QIODevice& device, const QString& msg )
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << msg;
    out.device()->seek(0);
    out << (quint16)(array.size() - sizeof(quint16));
    device.write(array);
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
