#include "PelicanProtocolTest.h"
#include "PelicanProtocol.h"
#include "ServerRequest.h"
#include "ServerResponse.h"
#include "AcknowledgementRequest.h"
#include "DataBlobResponse.h"
#include "ServiceDataRequest.h"
#include "StreamDataRequest.h"
#include "StreamDataResponse.h"
#include "ServiceDataResponse.h"
#include "StreamData.h"

#include "pelican/comms/DataSupportRequest.h"
#include "pelican/comms/DataSupportResponse.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/utility/test/SocketTester.h"
#include "pelican/data/test/TestDataBlob.h"
#include "pelican/server/WritableData.h"

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>

#include <iostream>
using std::endl;
using std::cout;
#include <cstdlib>

namespace pelican {

using test::TestDataBlob;
using test::SocketTester;

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
        DataChunk d1("d1",data1.data(),data1.size());
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
        buf1[d1.size()] = '\0';
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
        DataChunk d1("d1",data1.data(),data1.size());
        d1.setId("d1id");
        data.append(&d1);

        QByteArray data2("data2");
        DataChunk d2("d2",data2.data(),data2.size());
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
        buf1[d1.size()] = '\0';
        CPPUNIT_ASSERT_EQUAL( (long)d1.size(), (long)socket.read( &buf1[0], d1.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(&buf1[0]));
        std::vector<char> buf2(d2.size()+1);
        buf2[d2.size()] = '\0';
        CPPUNIT_ASSERT_EQUAL( (long)d2.size(), (long)socket.read( &buf2[0], d2.size() ) );
        CPPUNIT_ASSERT_EQUAL( std::string(data2.data()), std::string(&buf2[0]) );
    }
}



void PelicanProtocolTest::test_sendDataBlob()
{
    try {
        // Use Case
        // DataBlob type known
        // expect safe transfer
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        QString streamName("teststream");

        TestDataBlob blob;
        blob.setData("testdata");
        PelicanProtocol proto;
        proto.send(stream, streamName, blob);
        CPPUNIT_ASSERT( ! block.isEmpty() );

        // send it over a TCP Socket
        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        if( resp->type() == ServerResponse::Error ) {
             CPPUNIT_FAIL( "Socket Error: " + resp->message().toStdString() );
        }
        // make sure we get the correct type back
        CPPUNIT_ASSERT( resp->type() == ServerResponse::Blob );
        DataBlobResponse* db = static_cast<DataBlobResponse*>(resp.get());
        CPPUNIT_ASSERT( blob.type() == db->blobClass() );
    } catch (const QString& e) {
        CPPUNIT_FAIL("Caught exception: " + e.toStdString());
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
        StreamData streamData("d1", data1.data(), data1.size());
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)streamData.size() );
        streamData.setId("testid");
        AbstractProtocol::StreamData_t data;
        data.append(&streamData);
        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);
        QTcpSocket& socket = _st->send(block);

        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        CPPUNIT_ASSERT( resp->type() == ServerResponse::StreamData );
        StreamDataResponse* sd2 = static_cast<StreamDataResponse*>(resp.get());
        CPPUNIT_ASSERT( streamData == *(sd2->streamData()) );

        // Check we have the actual data.
        QByteArray buf(streamData.size(), 0);
        CPPUNIT_ASSERT_EQUAL( (long)streamData.size(), (long)socket.read( buf.data(), streamData.size() ));
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(buf.data() ));
    }
    {
        // Use Case
        // Single Stream Data with  service data
        PelicanProtocol proto;
        QByteArray data1("data1");
        StreamData sd("d1",data1.data(),data1.size());
        sd.setId("streamdataid");
        QByteArray data2("data2");
        boost::shared_ptr<DataChunk> d1(new DataChunk("d1",data2.data(),data2.size()) );
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
        QByteArray buf(sd.size(), 0);
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
        // A DataSupportRequest
        DataSupportRequest req;
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

void PelicanProtocolTest::test_sendDataSupport()
{
    {
        // Use Case:
        // Empty Stream and Service data
        // Expect to get a valid response object
        PelicanProtocol proto;
        QSet<QString> streams;
        DataSupportResponse data(streams);

        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);

        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        CPPUNIT_ASSERT( resp->type() == ServerResponse::DataSupport);
        DataSupportResponse* d = static_cast<DataSupportResponse*>(resp.get());
        CPPUNIT_ASSERT( d->streamData().size() == 0 );
    }
    {
        // Use Case:
        // Stream data only
        // Expect transfer of stream data
        PelicanProtocol proto;
        QSet<QString> streams;
        streams.insert("test1");
        streams.insert("test2");
        streams.insert("test3");
        DataSupportResponse data(streams);

        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);

        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        CPPUNIT_ASSERT( resp->type() == ServerResponse::DataSupport);
        DataSupportResponse* d = static_cast<DataSupportResponse*>(resp.get());
        CPPUNIT_ASSERT_EQUAL( 3, d->streamData().size() );
        CPPUNIT_ASSERT( d->streamData() == streams );
    }
    {
        // Use Case:
        // Stream data and service stream
        // Expect transfer of all service and stream data
        PelicanProtocol proto;
        QSet<QString> streams;
        streams.insert("test1");
        streams.insert("test2");
        streams.insert("test3");
        QSet<QString> service;
        service.insert("service_test1");
        service.insert("service test2");
        DataSupportResponse data( streams, service );

        QByteArray block;
        QBuffer stream(&block);
        stream.open(QIODevice::WriteOnly);
        proto.send(stream, data);

        QTcpSocket& socket = _st->send(block);
        boost::shared_ptr<ServerResponse> resp = _protocol.receive(socket);
        CPPUNIT_ASSERT( resp->type() == ServerResponse::DataSupport);
        DataSupportResponse* d = static_cast<DataSupportResponse*>(resp.get());
        CPPUNIT_ASSERT_EQUAL( 3, d->streamData().size() );
        CPPUNIT_ASSERT_EQUAL( 2, d->serviceData().size() );
    }
}


void PelicanProtocolTest::test_sendChunk()
{
    // Create some data.
    unsigned nData = 8000;
    std::vector<float> fData(nData);
    for (unsigned i = 0; i < nData; ++i) {
        fData[i] = float(i);
    }

    // Create a stream data object from the data.
    QString streamName = "testStream";
    QString streamId = "testId";
    StreamData sData(streamName, reinterpret_cast<void*>(&fData[0]),
            nData * sizeof(float));
    sData.setId(streamId);
    CPPUNIT_ASSERT_EQUAL(nData * sizeof(float), sData.size());
    CPPUNIT_ASSERT(sData.isValid());

    // Add the stream data to a stream data set.
    AbstractProtocol::StreamData_t streamDataSet;
    streamDataSet.append(&sData);

    // Create a pelican protocol used to send data.
    PelicanProtocol proto;

    // Create a device to send to.
    QByteArray buffer;
    QBuffer device(&buffer);
    device.open(QIODevice::WriteOnly);

    // Send data to the device.
    proto.send(device, streamDataSet);

    // Open the device for reading.
    device.close();
    device.open(QIODevice::ReadOnly);

    // Receive out of the device in the same was as
    // PelicanClientProtocol::receive()
    ServerResponse::Response type;
    QDataStream in(&device);
    in.setVersion(QDataStream::Qt_4_0);
    quint16 tmp;
    in >> (quint16&)tmp;
    type=(ServerResponse::Response)tmp;
    CPPUNIT_ASSERT_EQUAL(ServerResponse::StreamData, type);
    boost::shared_ptr<StreamDataResponse> receiveResp(new StreamDataResponse);
    quint16 streams;
    in >> streams;
    CPPUNIT_ASSERT_EQUAL(quint16(1), streams);
    QString name;
    in >> name;
    QString id;
    in >> id;
    quint64 size;
    in >> size;
    CPPUNIT_ASSERT_EQUAL(streamName.toStdString(), name.toStdString());
    CPPUNIT_ASSERT_EQUAL(streamId.toStdString(), id.toStdString());
    quint64 expectedSize = nData * sizeof(float);
    CPPUNIT_ASSERT_EQUAL(expectedSize, size);

    quint16 assocaites;
    in >> assocaites;
    CPPUNIT_ASSERT_EQUAL(quint16(0), assocaites);

    StreamData * outStream = new StreamData(name, 0, (unsigned long)size);
    receiveResp->setStreamData(outStream);
    outStream->setId(id);

    // Copy of content of PelicanServerClient::_response()
    QHash<QString, DataBlob*> validData;
    CPPUNIT_ASSERT_EQUAL(ServerResponse::StreamData, receiveResp->type());

    StreamDataResponse* response = static_cast<StreamDataResponse*>(receiveResp.get());
    StreamData * sd = response->streamData();
    CPPUNIT_ASSERT(sd != 0);

    // _adaptStream(device, sd, dataHash)
    CPPUNIT_ASSERT_EQUAL(size_t(nData * sizeof(float)), sd->size());

    // adapt and check.
    std::vector<char> outDataTemp(int(nData * sizeof(float)));
    device.read(&outDataTemp[0], nData * sizeof(float));
    std::vector<float> outData(nData);
    float* out = &outData[0];
    out = reinterpret_cast<float*>(&outDataTemp[0]);
    for (unsigned i = 0; i < nData; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(float(i), out[i], 1e-5f);
    }
}



PelicanProtocolTest::Socket_t& PelicanProtocolTest::_send(ServerRequest* req)
{
    // create a Socket
    Socket_t& socket = _st->send( _protocol.serialise(*req) );
    return socket;
}




} // namespace pelican
