#include "pelican/comms/PelicanClientProtocol.h"
#include "pelican/comms/DataChunk.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/comms/ServerResponse.h"
#include "pelican/comms/ServiceDataResponse.h"
#include "pelican/comms/DataBlobResponse.h"
#include "pelican/comms/StreamDataResponse.h"
#include "pelican/comms/DataSupportResponse.h"
#include "pelican/comms/ServiceDataRequest.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

#include <QtNetwork/QAbstractSocket>

#include <iostream>

namespace pelican {

PelicanClientProtocol::PelicanClientProtocol()
    : AbstractClientProtocol()
{
}

PelicanClientProtocol::~PelicanClientProtocol()
{
}

QByteArray PelicanClientProtocol::serialise(const ServerRequest& req)
{
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_0);
    ds << (quint16)req.type();

    switch(req.type())
    {
        case ServerRequest::Acknowledge:
            break;
        case ServerRequest::DataSupport:
            break;
        case ServerRequest::StreamData:
        {
            const StreamDataRequest& r = static_cast<const StreamDataRequest&>(req);
            ds << (quint16)r.size();
            DataRequirementsIterator it=r.begin();
            while( it != r.end() )
            {
                _serializeDataRequirements(ds, *it);
                ++it;
            }
            break;
        }
        case ServerRequest::ServiceData:
        {
            const ServiceDataRequest& r = static_cast<const ServiceDataRequest&>(req);
            ds << (quint16)r.types().size();
            foreach( QString type, r.types() ) {
                ds << type << r.version(type);
            }
            break;
        }
        default:
            break;
    }
    return data;
}


boost::shared_ptr<ServerResponse> PelicanClientProtocol::receive(QAbstractSocket& socket)
{
    int timeout = 2000;
    ServerResponse::Response type = ServerResponse::Error;
    while (socket.bytesAvailable() < (int)sizeof(quint16)) {
        if ( !socket.waitForReadyRead(timeout) ) {
            std::cout << "PelicanClientProtocol: Receive error!" << std::endl;
            return boost::shared_ptr<ServerResponse>(new ServerResponse(type,
                    socket.errorString()));
        }
    }

    QDataStream in(&socket);
    quint16 tmpType;
    in.setVersion(QDataStream::Qt_4_0);
    in >> (quint16&)tmpType;
    type = (ServerResponse::Response)tmpType;

    switch(type)
    {
        case ServerResponse::Acknowledge:
            break;

        case ServerResponse::DataSupport:
        {
            QSet<QString> streams;
            QSet<QString> services;
            in >> streams;
            in >> services;
            boost::shared_ptr<DataSupportResponse> s(new DataSupportResponse(
                    streams, services));
            return s;
            break;
        }

        case ServerResponse::StreamData:
        {
            boost::shared_ptr<StreamDataResponse> s(new StreamDataResponse);
            quint16 streams;
            in >> streams;
            for (int i=0; i < streams; ++i)
            {
                QString name;
                in >> name;
                QString id;
                in >> id;
                quint64 size;
                in >> size;

                StreamData* sd = new StreamData(name, 0, (unsigned long)size);
                s->setStreamData(sd);
                sd->setId(id);

                // read in associate meta-data
                quint16 associates;
                in >> associates;
                for (unsigned j = 0; j < associates; ++j) {
                    in >> name >> id >> size;
                    sd->addAssociatedData( boost::shared_ptr<DataChunk>(
                            new DataChunk(name, id, size)));
                }
            }
            return s;
            break;
        }

        case ServerResponse::ServiceData:
        {
            boost::shared_ptr<ServiceDataResponse> s(new ServiceDataResponse);
            quint16 sets;
            in >> sets;
            QString name;
            QString version;
            quint64 size;
            for(int i=0; i < sets; ++i ) {
                in >> name;
                in >> version;
                in >> size;
                s->addData(new DataChunk(name, version, size));
            }
            return s;
            break;
        }

        case ServerResponse::Blob:
        {
            QString type;
            in >> type;
            QString name;
            in >> name;
            quint64 dataSize;
            in >> dataSize;
            boost::shared_ptr<DataBlobResponse> s(
                    new DataBlobResponse(type, name, dataSize,
                            (QSysInfo::Endian)in.byteOrder()));
            return s;
            break;
        }

        default:
            break;
    }

    return boost::shared_ptr<ServerResponse>(
            new ServerResponse(ServerResponse::Error,
                    QString("PelicanClientProtocol: Unknown type passed: %1")
                    .arg(type)));
}


void PelicanClientProtocol::_serializeDataRequirements(QDataStream& stream,
        const DataRequirements& req) const
{
    QSet<QString> serviceData = req.serviceData();
    QSet<QString> streamData = req.streamData();
    stream << serviceData << streamData;
}

} // namespace pelican
