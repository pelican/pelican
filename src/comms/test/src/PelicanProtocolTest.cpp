#include "PelicanProtocolTest.h"
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "AcknowledgementRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "data/DataRequirements.h"
#include "utility/SocketTester.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanProtocolTest );
// class PelicanProtocolTest 
PelicanProtocolTest::PelicanProtocolTest()
    : CppUnit::TestFixture()
{
}

PelicanProtocolTest::~PelicanProtocolTest()
{
}

void PelicanProtocolTest::setUp()
{
    _st = new SocketTester;
}

void PelicanProtocolTest::tearDown()
{
    delete _st;
}

void PelicanProtocolTest::test_request()
{
    {
        // Use Case:
        // An Acknowledgement Request
        AcknowledgementRequest req;
        PelicanProtocol proto;
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == *(proto.request(socket)) );
    }
    {
        // Use Case:
        // An empty ServiceData Request
        PelicanProtocol proto;
        ServiceDataRequest req;
        Socket_t& socket = _send(&req);
        boost::shared_ptr<ServerRequest> req2 = proto.request(socket);
        CPPUNIT_ASSERT( req == *req2 );
    }
    {
        // Use Case:
        // A serviceData Request with 2 objects
        PelicanProtocol proto;
        ServiceDataRequest req;
        req.request("testa","versiona");
        req.request("testb","versionb");
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == *(proto.request(socket)) );
    }
    {
        // Use Case:
        // An empty StreamData Request
        StreamDataRequest req;
        PelicanProtocol proto;
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == *(proto.request(socket)) );
    }
    {
        // Use Case:
        // A non-empty StreamData Request
        StreamDataRequest req;
        DataRequirements require1;
        require1.setStreamData("teststream");
        require1.setServiceData("testservice");
        DataRequirements require2;
        require2.setStreamData("teststream2");
        require2.setServiceData("testservice2");
        PelicanProtocol proto;
        req.addDataOption(require1);
        req.addDataOption(require2);
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == *(proto.request(socket)) );
    }
}

PelicanProtocolTest::Socket_t& PelicanProtocolTest::_send(ServerRequest* req)
{
    // create a Socket
    Socket_t& socket = _st->send( _protocol.serialise(*req) );
    return socket;
}

} // namespace pelican
