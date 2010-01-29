#include "PelicanProtocolTest.h"
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "AcknowledgementRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
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
        CPPUNIT_ASSERT( req == proto.request(socket) );
    }
    {
        // Use Case:
        // A ServiceData Request
        PelicanProtocol proto;
        ServiceDataRequest req;
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == proto.request(socket) );
    }
    {
        // Use Case:
        // A StreamData Request
        StreamDataRequest req;
        PelicanProtocol proto;
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == proto.request(socket) );
    }
}

PelicanProtocolTest::Socket_t& PelicanProtocolTest::_send(ServerRequest* req)
{
    // create a Socket
    Socket_t& socket = _st->send( _protocol.serialise(*req) );
    return socket;
}

} // namespace pelican
