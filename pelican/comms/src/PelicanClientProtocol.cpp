/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "comms/PelicanClientProtocol.h"
#include "comms/DataChunk.h"
#include "comms/StreamData.h"
#include "comms/ServerRequest.h"
#include "comms/ServerResponse.h"
#include "comms/ServiceDataResponse.h"
#include "comms/DataBlobResponse.h"
#include "comms/StreamDataResponse.h"
#include "comms/DataSupportResponse.h"
#include "comms/ServiceDataRequest.h"
#include "comms/StreamDataRequest.h"
#include "data/DataSpec.h"
#include "data/DataBlob.h"

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
            DataSpecIterator it=r.begin();
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
    int timeout = 1000;
    ServerResponse::Response type = ServerResponse::Error;
    while (socket.bytesAvailable() < (int)sizeof(quint16)) {
        if ( !socket.waitForReadyRead(timeout) ) {
            std::cerr << "PelicanClientProtocol: Receive error: "
                      << socket.errorString().toStdString() << std::endl;
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
            DataSpec spec;
            spec.addServiceData( services );
            spec.addStreamData( streams );
            QHash<QString, QString> adapters;
            in >> adapters;
            spec.addAdapterTypes( adapters );
            boost::shared_ptr<DataSupportResponse> s(new DataSupportResponse(spec));
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
        const DataSpec& req) const
{
    QSet<QString> serviceData = req.serviceData();
    QSet<QString> streamData = req.streamData();
    stream << serviceData << streamData;
}

} // namespace pelican
