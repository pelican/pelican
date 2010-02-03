#include "PelicanServerClient.h"
#include "data/DataRequirements.h"
#include "comms/StreamDataRequest.h"
#include "comms/ServiceDataRequest.h"
#include "comms/PelicanClientProtocol.h"
#include <QHash>
#include <QTcpSocket>


#include "utility/memCheck.h"

namespace pelican {


// class PelicanServerClient 
PelicanServerClient::PelicanServerClient()
    : AbstractDataClient(), _protocol(0)
{
    _protocol = new PelicanClientProtocol; // hard code for now
}

PelicanServerClient::~PelicanServerClient()
{
    delete _protocol;
}

QHash<QString, DataBlob*> PelicanServerClient::getData(const DataRequirements& req)
{
    // construct the request
    StreamDataRequest sr;
    sr.addDataOption(req);

    // send the request to the server
    QTcpSocket sock;
    //sock.connectToHost( _server, _port );
    //sock.send( _protocol.serialise(*req) );
    //sock.flush();

    // interpret server response
    _protocol->receive(sock);

}

void PelicanServerClient::_connect()
{
}


} // namespace pelican
