#include "TestServer.h"
#include "PelicanPortServer.h"
#include "StreamDataBuffer.h"
#include "ServiceDataBuffer.h"
#include "WritableData.h"
#include "LockableData.h"
#include "comms/PelicanProtocol.h"
#include "comms/StreamData.h"
#include "comms/Data.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details TestServer 
   The default portocol is the standard PelicanProtocol, which can be
   overridden in the constructor.
 */
TestServer::TestServer(AbstractProtocol* proto)
    : _protoOwner(false), _proto(proto)
{
    // set up the protocol
    if(_proto) {
        _proto = new PelicanProtocol;
        _protoOwner = true;
    }
    _server = new PelicanPortServer(_proto,&_dm);
    _server->listen(QHostAddress::Any, 0 );
}

/**
 *@details
 */
TestServer::~TestServer()
{
    if(_protoOwner)
        delete _proto;
}

quint16 TestServer::port() const
{
    return _server->serverPort();
}


void TestServer::serveStreamData(const QList<StreamData>& data)
{
    foreach( StreamData d, data )
    {
        StreamDataBuffer* buf = _dm.getStreamBuffer(d.name()); // ensures the StreamData buffer exists
        WritableData wd = buf->getWritable(d.size());
        *(wd.data()->data()) = d;
    }
}

void TestServer::serveServiceData(const QList<Data>& data )
{
    foreach( Data d, data )
    {
        ServiceDataBuffer* buf = _dm.getServiceBuffer(d.name()); // ensures the ServiceData buffer exists
        WritableData wd = buf->getWritable(d.size());
        *(wd.data()->data()) = d;
    }
}

} // namespace pelican
