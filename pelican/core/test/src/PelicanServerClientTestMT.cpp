#include "PelicanServerClientTestMT.h"
#include "PelicanServerClient.h"
#include "pelican/server/test/TestServer.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/DataChunk.h"
#include "pelican/core/test/TestServiceAdapter.h"
#include "pelican/core/test/TestStreamAdapter.h"
#include "pelican/data/test/TestDataBlob.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

#include <QtCore/QCoreApplication>

namespace pelican {

using test::TestDataBlob;
using test::TestServiceAdapter;
using test::TestStreamAdapter;
using test::TestServer;

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanServerClientTestMT );
/**
 *@details PelicanServerClientTestMT
 */
PelicanServerClientTestMT::PelicanServerClientTestMT()
    : CppUnit::TestFixture()
{
     int argc = 1;
     char *argv[] = {(char*)"pelican"};
     _coreApp = new QCoreApplication(argc, argv);
}

/**
 *@details
 */
PelicanServerClientTestMT::~PelicanServerClientTestMT()
{
}

void PelicanServerClientTestMT::setUp()
{
}

void PelicanServerClientTestMT::tearDown()
{
}

void PelicanServerClientTestMT::test_getData()
{
    try{
    // set up the test server
    TestServiceAdapter serviceAdapter;
    TestStreamAdapter streamAdapter;
    TestServer server;
    QString stream1("stream1");
    QString service1("service1");
    QString version1("version1");
    QString version2("version2");
    DataRequirements reqStream1;
    reqStream1.addStreamData(stream1);
    QByteArray data1("pelican/data1");
    QByteArray data2("pelican/data2");
    unsigned int port = server.port();

    // common client configuration
    Config config;
    config.setFromString(
        "",
        "<testconfig>"
        "   <server host=\"127.0.0.1\"/>"
        "</testconfig>"
    );
    Config::TreeAddress address;
    address << Config::NodeId("server", "");
    address << Config::NodeId("testconfig", "");
    ConfigNode configNode = config.get(address);
    {
        // Use Case:
        // request for Service Data only
        // Expect :
        // throw
        DataChunk sd(service1, version1, data1 );
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );

        // setup the test
        DataRequirements req;
        req.addServiceData(service1);
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(service1, &serviceAdapter);
        PelicanServerClient client(configNode, dt, 0);
        client.setPort(port);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db("DataBlob");
        dataHash.insert(service1, &db);
        CPPUNIT_ASSERT_THROW( client.getData(dataHash), QString );
    }
    {
        // Use Case:
        // Single Request for an existing stream dataset
        // no service data
        // Expect:
        // return the required data stream
        StreamData sd(stream1, version1, data1 );
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
        server.serveStreamData(sd);

        // setup the test
        QList<DataRequirements> lreq;
        lreq.append(reqStream1);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(stream1, &streamAdapter);
        PelicanServerClient client(configNode, dt, 0);
        client.setPort(port);

        QHash<QString, DataBlob*> dataHash;
        TestDataBlob db;
        dataHash.insert(stream1, &db);
        client.getData(dataHash);
        CPPUNIT_ASSERT_EQUAL(version1.toStdString(), db.version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(db.data()) );
    }
    {
        // Use Case:
        // Single Request for an existing stream dataset
        // with existing service data
        // Expect:
        // return the required data stream
        DataChunk servd(service1, version2, data2 );
        server.serveServiceData(servd);
        StreamData sd(stream1, version1, data1 );
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
        server.serveStreamData(sd);

        // setup the test
        DataRequirements req;
        req.addServiceData(service1);
        req.addStreamData(stream1);
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(stream1, &streamAdapter);
        dt.setAdapter(service1, &serviceAdapter);
        PelicanServerClient client(configNode, dt, 0);
        client.setPort(port);

        QHash<QString, DataBlob*> dataHash;
        TestDataBlob db;
        TestDataBlob db_service;
        dataHash.insert(stream1, &db);
        dataHash.insert(service1, &db_service);
        client.getData(dataHash);
        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), db.version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( version2.toStdString(), db_service.version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string(data2.data()), std::string(db_service.data()) );
        CPPUNIT_ASSERT_EQUAL( std::string(data1.data()), std::string(db.data()) );
    }
    }
    catch(const QString& e)
    {
        std::cerr << "error: uncaught exception" << e.toStdString();
        throw e;
    }
}

} // namespace pelican
