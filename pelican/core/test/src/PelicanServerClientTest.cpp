#include "PelicanServerClientTest.h"
#include "PelicanServerClient.h"
#include "pelican/data/DataRequirements.h"

#include "pelican/data/test/TestDataBlob.h"
#include "pelican/core/test/TestServiceAdapter.h"
#include "pelican/core/test/TestStreamAdapter.h"

#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/test/SocketTester.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/DataChunk.h"
#include "pelican/comms/StreamDataResponse.h"
#include "pelican/comms/ServiceDataResponse.h"
#include "pelican/server/test/TestServer.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtXml/QDomElement>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpSocket>

namespace pelican {

using test::TestDataBlob;
using test::SocketTester;
using test::TestServiceAdapter;
using test::TestStreamAdapter;

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanServerClientTest );

/**
 *@details PelicanServerClientTest
 */
PelicanServerClientTest::PelicanServerClientTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
PelicanServerClientTest::~PelicanServerClientTest()
{
}

void PelicanServerClientTest::setUp()
{
}

void PelicanServerClientTest::tearDown()
{
}

void PelicanServerClientTest::test_getData()
{
    Config config;
    config.setFromString(
            "<testconfig>"
            "   <server host=\"127.0.0.1\"/>"
            "</testconfig>"
            );
    Config::TreeAddress address;
    address << Config::NodeId("testconfig", "");
    ConfigNode configNode = config.get(address);
    {
        // Use Case:
        //    Empty data requirements
        // Expect: throw with a suitable complaint

        // setup the test
        QList<DataRequirements> lreq;
        DataTypes dt;
        dt.addData(lreq);
        PelicanServerClient* client = 0;
        CPPUNIT_ASSERT_THROW(client = new PelicanServerClient(configNode, dt, 0), QString);
        delete client;
    }
    TestStreamAdapter streamAdapter;
    QString stream1("stream1");
    QString version1("version1");
    DataRequirements req;
    req.addStreamData(stream1);
    {
        // Use Case:
        //    data requirements are set, empty hash passed
        // Expect: throw

        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(stream1,&streamAdapter);
        PelicanServerClient client(configNode, dt, 0);
        QHash<QString, DataBlob*> dataHash;
        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString );
    }
    {
        // Use Case:
        //    Data Requirements do not match DataBlobs
        // Expect:
        //     throw with a suitable message

        // setup the test
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(stream1,&streamAdapter);
        PelicanServerClient client(configNode, dt, 0);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db("DataBlob");
        dataHash.insert("pelican/data2", &db);

        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString);
    }
}

void PelicanServerClientTest::test_response()
{
    TestServiceAdapter serviceAdapter;
    TestStreamAdapter streamAdapter;
    ConfigNode configNode;
    configNode.setFromString(
            "<testconfig>"
            "   <server host=\"127.0.0.1\"/>"
            "</testconfig>"
    );
    QString stream1("stream1");
    QString version1("v1");
    QString service1("service1");
    QString serviceVersion1("sv1");
    DataRequirements reqStream1;
    reqStream1.addStreamData(stream1);
    QByteArray data1("data1");
    {
        // Use Case
        // receive a StreamData response (single stream, no service data)
        boost::shared_ptr<ServerResponse> res( new StreamDataResponse );
        static_cast<StreamDataResponse*>(res.get())->setStreamData( new StreamData(stream1,version1, data1.size()) );

        // set up the adapters and client
        QList<DataRequirements> lreq;
        lreq.append(reqStream1);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(stream1,&streamAdapter);
        PelicanServerClient client(configNode, dt, 0);

        // set up the memory for receiving data
        TestDataBlob db;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(stream1, &db);

        SocketTester st;
        QTcpSocket& sock = st.send(data1);
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), vhash[stream1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data1.data() ) , std::string( static_cast<TestDataBlob*>(vhash[stream1])->data() ) );
    }
    QByteArray data2("data2");
    {
        // Use Case
        // ServiceData response - single service data
        boost::shared_ptr<ServerResponse> res( new ServiceDataResponse );
        static_cast<ServiceDataResponse*>(res.get())->addData( new DataChunk(service1,serviceVersion1, data2.size()) );

        // set up the memory for receiving data
        TestDataBlob db;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(service1, &db);

        // set up the adapters and client
        DataRequirements req;
        req.addStreamData(service1);
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(service1,&serviceAdapter);
        PelicanServerClient client(configNode, dt, 0);

        SocketTester st;
        QTcpSocket& sock = st.send(data2);
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( serviceVersion1.toStdString(), vhash[service1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data2.data() ) , std::string( static_cast<TestDataBlob*>(vhash[service1])->data() ) );
    }
    QString service2("service2");
    {
        // Use Case
        // ServiceData response - multiple service data
        boost::shared_ptr<ServerResponse> res( new ServiceDataResponse );
        static_cast<ServiceDataResponse*>(res.get())->addData( new DataChunk(service1,version1, data1.size()) );
        static_cast<ServiceDataResponse*>(res.get())->addData( new DataChunk(service2,serviceVersion1, data2.size()) );

        // set up the memory for receiving data
        TestDataBlob db;
        TestDataBlob db2;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(service1, &db);
        dataHash.insert(service2, &db2);

        // set up the adapters and client
        DataRequirements req;
        req.addStreamData(service1);
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(service1,&serviceAdapter);
        dt.setAdapter(service2,&serviceAdapter);
        PelicanServerClient client(configNode, dt, 0);

        SocketTester st;
        QTcpSocket& sock = st.send( data1 + data2 );
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), vhash[service1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( serviceVersion1.toStdString(), vhash[service2]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data1.data() ) , std::string( static_cast<TestDataBlob*>(vhash[service1])->data() ) );
        CPPUNIT_ASSERT_EQUAL( std::string( data2.data() ) , std::string( static_cast<TestDataBlob*>(vhash[service2])->data() ) );
    }

//    {
//        // Use Case
//        // receive a StreamData response with associated service data (single stream, single service data)
//        // no existing service data exists
//        boost::shared_ptr<ServerResponse> res( new StreamDataResponse );
//        StreamData* sd = new StreamData(stream1,version1, data1.size());
//        sd->addAssociatedData(boost::shared_ptr<Data>(new Data(service1,serviceVersion1, data2.size()) ) );
//        static_cast<StreamDataResponse*>(res.get())->setStreamData( sd );
//
//        // set up the adapters and client
//        DataRequirements req;
//        req.addStreamData(stream1);
//        req.addServiceData(service1);
//        QList<DataRequirements> lreq;
//        lreq.append(req);
//        DataTypes dt;
//        dt.addData(lreq);
//        dt.setAdapter(stream1,&streamAdapter);
//        dt.setAdapter(service1,&serviceAdapter);
//        PelicanServerClient client(configNode, dt, 0);
//
//        // set up the memory for receiving data
//        TestDataBlob db;
//        TestDataBlob db2;
//        QHash<QString, DataBlob*> dataHash;
//        dataHash.insert(stream1, &db);
//        dataHash.insert(service1, &db2);
//
//        SocketTester st;
//        QTcpSocket& sock = st.send(data1);
//        try {
//            QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
//        } catch (const QString& e) {
//            CPPUNIT_FAIL(e.toStdString());
//        }
//        CPPUNIT_ASSERT( vhash == dataHash );
//        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), vhash[stream1]->version().toStdString() );
//        CPPUNIT_ASSERT_EQUAL( serviceVersion1.toStdString(), vhash[service1]->version().toStdString() );
//        CPPUNIT_ASSERT_EQUAL( std::string( data1.data() ) , std::string( static_cast<TestDataBlob*>( vhash[stream1])->data() ) );
//        CPPUNIT_ASSERT_EQUAL( std::string( data2.data() ) , std::string( static_cast<TestDataBlob*>( vhash[service1])->data() ) );
//    }

}

} // namespace pelican
