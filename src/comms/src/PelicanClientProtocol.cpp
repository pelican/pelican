#include "PelicanClientProtocol.h"
#include "ServerRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "data/DataRequirements.h"
#include <QByteArray>
#include <QDataStream>
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


// class PelicanClientProtocol 
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
    switch(req.type()) {
        case ServerRequest::Acknowledge:
            break;
        case ServerRequest::DataSupport:
            break;
        case ServerRequest::StreamData:
            {
                const StreamDataRequest& r = static_cast<const StreamDataRequest& >(req);
                ds << (quint16)r.size();
                DataRequirementsIterator it=r.begin();
                while( it != r.end() )
                {
                    _serializeDataRequirements(ds, *it);
                    ++it;
                }
            }
            break;
        case ServerRequest::ServiceData:
            {
                const ServiceDataRequest& r = static_cast<const ServiceDataRequest& >(req);
                ds << (quint16)r.types().size();
                foreach( QString type, r.types() ) {
                    ds << type << r.version(type);
                }
            }
            break;
        default:
            break;
    }
    return data;
}

void PelicanClientProtocol::_serializeDataRequirements(QDataStream& stream, const DataRequirements& req) const
{
    QSet<QString> serviceData = req.serviceData();
    QSet<QString> streamData = req.streamData();
    stream << serviceData << streamData;

}

} // namespace pelican
