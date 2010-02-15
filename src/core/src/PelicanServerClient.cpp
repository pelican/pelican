#include "PelicanServerClient.h"
#include "data/DataRequirements.h"
#include "comms/ServerResponse.h"
#include "comms/StreamDataRequest.h"
#include "comms/ServiceDataRequest.h"
#include "comms/StreamDataResponse.h"
#include "comms/ServiceDataResponse.h"
#include "comms/PelicanClientProtocol.h"
#include <QHash>
#include <QTcpSocket>

#include "utility/memCheck.h"

namespace pelican {


// class PelicanServerClient 
PelicanServerClient::PelicanServerClient(const ConfigNode& config,
        AdapterFactory* adapterFactory,
        QList<DataRequirements>& dataRequirements)
    : AbstractDataClient(config, adapterFactory, dataRequirements), _protocol(0)
{
    _protocol = new PelicanClientProtocol; // hard code for now
}

PelicanServerClient::~PelicanServerClient()
{
    delete _protocol;
}

QHash<QString, DataBlob*> PelicanServerClient::getData(QHash<QString, DataBlob*>& dataHash)
{
    QHash<QString, DataBlob*> validData;

    // construct the request
    StreamDataRequest sr;
//    sr.addDataOption(dataRequirements()); // FIXME

    // send the request to the server
    QTcpSocket sock;
    //sock.connectToHost( _server, _port );
    //sock.send( _protocol.serialise(*req) );
    //sock.flush();

    // interpret server response
    boost::shared_ptr<ServerResponse> r = _protocol->receive(sock);
    switch( r->type() ) {
        case ServerResponse::Error:
            break;
        case ServerResponse::StreamData:
            {
                // manage the service data
            }
            break;
        default:
            break;
    }

    return validData;
}

void PelicanServerClient::_connect()
{
}

void PelicanServerClient::getServiceData(QHash<QString,QString> requirements, QHash<QString, DataBlob*>& datahash)
{
    StreamDataRequest sr;
}

} // namespace pelican
