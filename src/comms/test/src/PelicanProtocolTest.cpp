#include "PelicanProtocolTest.h"
#include "data/ServerRequest.h"
#include "data/AcknowledgementRequest.h"
#include "data/ServiceDataRequest.h"
#include "data/StreamDataRequest.h"
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
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == _protocol.request(socket) );
    }
    {
        // Use Case:
        // A ServiceData Request
        ServiceDataRequest req;
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == _protocol.request(socket) );
    }
    {
        // Use Case:
        // A StreamData Request
        StreamDataRequest req;
        Socket_t& socket = _send(&req);
        CPPUNIT_ASSERT( req == _protocol.request(socket) );
    }
}

PelicanProtocolTest::Socket_t& PelicanProtocolTest::_send(ServerRequest* req)
{
    // create a Socket
    Socket_t socket = _st->send( req->serialise() );
    return socket;
}

} // namespace pelican
