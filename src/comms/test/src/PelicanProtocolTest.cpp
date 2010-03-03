#include <QBuffer>
#include "PelicanProtocolTest.h"
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "ServerResponse.h"
#include "AcknowledgementRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "StreamDataResponse.h"
#include "StreamData.h"
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

void PelicanProtocolTest::test_sendStreamData()
{
    {
        // Use Case
        // Empty Data
        PelicanProtocol proto;
        AbstractProtocol::StreamData_t data;
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        CPPUNIT_ASSERT( ! block.isEmpty() );
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(_st->send(block));
        if( resp->type() == ServerResponse::Error ) {
             CPPUNIT_FAIL( "Socket Error: " + resp->message().toStdString() );
        }
        CPPUNIT_ASSERT( resp->type() == ServerResponse::StreamData );
    }
    {
        // Use Case
        // Single Stream Data with no service data
        PelicanProtocol proto;
        StreamData sd("test");
        sd.setId("testid");
        AbstractProtocol::StreamData_t data;
        data.insert(QString("teststream"), &sd);
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(_st->send(block));
        CPPUNIT_ASSERT( resp->type() == ServerResponse::StreamData );
        StreamDataResponse* sd2 = static_cast<StreamDataResponse*>(resp.get());
        CPPUNIT_ASSERT( sd == *(sd2->streamData()) );
    }
    /*
    {
        // Use Case
        // Single Stream Data with  service data
        PelicanProtocol proto;
        StreamData sd("test");
        Data d1("d1",0,100);
        d1.setId("d1id");
        sd.addAssociatedData(&d1);

        AbstractProtocol::StreamData_t data;
        data.insert(QString("teststream"), &sd);
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(_st->send(block));
        CPPUNIT_ASSERT( resp->type() == ServerResponse::StreamData );
        StreamDataResponse* sd2 = static_cast<StreamDataResponse*>(resp.get());
        CPPUNIT_ASSERT( sd == *(sd2->streamData()) );
    }
    */
}

void PelicanProtocolTest::test_request()
{
    // Request Processing tests
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
