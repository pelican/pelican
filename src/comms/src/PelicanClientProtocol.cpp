#include "PelicanClientProtocol.h"
#include "ServerRequest.h"
#include <QByteArray>

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
    switch(req.type()) {
        case ServerRequest::Acknowledge:
            break;
        case ServerRequest::DataSupport:
            break;
        case ServerRequest::StreamData:
            {

            }
            break;
        case ServerRequest::ServiceData:
            break;
        default:
            break;
    }
    return data;
}

} // namespace pelican
