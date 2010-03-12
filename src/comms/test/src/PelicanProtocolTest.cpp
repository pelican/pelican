#include <QBuffer>
#include "PelicanProtocolTest.h"
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "ServerResponse.h"
#include "AcknowledgementRequest.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "StreamDataResponse.h"
#include "ServiceDataResponse.h"
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

void PelicanProtocolTest::test_sendServiceData()
{
    {
        // Use Case
        // Empty Data
        PelicanProtocol proto;
        AbstractProtocol::ServiceData_t data;
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        CPPUNIT_ASSERT( ! block.isEmpty() );
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(_st->send(block));
        if( resp->type() == ServerResponse::Error ) {
             CPPUNIT_FAIL( "Socket Error: " + resp->message().toStdString() );
        }
        CPPUNIT_ASSERT( resp->type() == ServerResponse::ServiceData );
        ServiceDataResponse* sd = static_cast<ServiceDataResponse*>(resp.get());
        CPPUNIT_ASSERT_EQUAL( 0, sd->data().size() );
    }
    {
        // Use Case
        // A Single Data Object
        PelicanProtocol proto;
        AbstractProtocol::ServiceData_t data;
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);

        QByteArray data1("data1");
        Data d1("d1",data1.data(),data1.size());
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)d1.size());
        d1.setId("d1id");
        data.append(&d1);
        proto.send(stream, data);

        CPPUNIT_ASSERT( ! block.isEmpty() );
        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        if( resp->type() == ServerResponse::Error ) {
             CPPUNIT_FAIL( "Socket Error: " + resp->message().toStdString() );
        }
        CPPUNIT_ASSERT( resp->type() == ServerResponse::ServiceData );
        ServiceDataResponse* sd = static_cast<ServiceDataResponse*>(resp.get());
        CPPUNIT_ASSERT_EQUAL( 1, sd->data().size() );
        CPPUNIT_ASSERT( d1 == *(sd->data()[0]) );
        std::vector<char> buf1(d1.size()+1);
        buf1[d1.size()+1] = '\0';
        CPPUNIT_ASSERT_EQUAL( (long)d1.size(), (long)socket.read( &buf1[0], d1.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(&buf1[0]));
    }
    {
        // Use Case
        // Multiple Data Objects
        PelicanProtocol proto;
        AbstractProtocol::ServiceData_t data;
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);

        QByteArray data1("data1");
        Data d1("d1",data1.data(),data1.size());
        d1.setId("d1id");
        data.append(&d1);

        QByteArray data2("data2");
        Data d2("d2",data2.data(),data2.size());
        d1.setId("d2id");
        data.append(&d2);
        proto.send(stream, data);

        CPPUNIT_ASSERT( ! block.isEmpty() );
        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        if( resp->type() == ServerResponse::Error ) {
             CPPUNIT_FAIL( "Socket Error: " + resp->message().toStdString() );
        }
        CPPUNIT_ASSERT( resp->type() == ServerResponse::ServiceData );
        ServiceDataResponse* sd = static_cast<ServiceDataResponse*>(resp.get());
        CPPUNIT_ASSERT_EQUAL( 2, sd->data().size() );
        CPPUNIT_ASSERT( d1 == *( sd->data()[0]) );
        CPPUNIT_ASSERT( d2 == *( sd->data()[1]) );

        // check we have the actual data
        CPPUNIT_ASSERT_EQUAL((long)data1.size(), (long)d1.size());
        std::vector<char> buf1(d1.size()+1);
        buf1[d1.size()+1] = '\0';
        CPPUNIT_ASSERT_EQUAL( (long)d1.size(), (long)socket.read( &buf1[0], d1.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(&buf1[0]));
        std::vector<char> buf2(d2.size()+1);
        buf2[d2.size()+1] = '\0';
        CPPUNIT_ASSERT_EQUAL( (long)d2.size(), (long)socket.read( &buf2[0], d2.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data2.data()), std::string(&buf2[0]) );
    }
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
        QByteArray data1("data1");
        StreamData sd("d1",data1.data(),data1.size());
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
        sd.setId("testid");
        AbstractProtocol::StreamData_t data;
        data.append(&sd);
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        CPPUNIT_ASSERT( resp->type() == ServerResponse::StreamData );
        StreamDataResponse* sd2 = static_cast<StreamDataResponse*>(resp.get());
        CPPUNIT_ASSERT( sd == *(sd2->streamData()) );

        // check we have the actual data
        QByteArray buf(0,sd.size());
        CPPUNIT_ASSERT_EQUAL( (long)sd.size(), (long)socket.read( buf.data(), sd.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(buf.data()));
    }
    {
        // Use Case
        // Single Stream Data with  service data
        PelicanProtocol proto;
        QByteArray data1("data1");
        StreamData sd("d1",data1.data(),data1.size());
        sd.setId("streamdataid");
        QByteArray data2("data2");
        boost::shared_ptr<Data> d1(new Data("d1",data2.data(),data2.size()) );
        d1->setId("d1id");
        sd.addAssociatedData(d1);

        AbstractProtocol::StreamData_t data;
        data.append(&sd);
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        CPPUNIT_ASSERT( resp->type() == ServerResponse::StreamData );
        StreamDataResponse* sd2 = static_cast<StreamDataResponse*>(resp.get());
        CPPUNIT_ASSERT( sd == *(sd2->streamData()) );

        // check we have the actual data
        QByteArray buf(0,sd.size());
        CPPUNIT_ASSERT_EQUAL( (long)sd.size(), (long)socket.read( buf.data(), sd.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(buf.data()));
    }
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
