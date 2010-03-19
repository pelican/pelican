#include <QCoreApplication>
#include "PelicanServerClientTestMT.h"
#include "PelicanServerClient.h"
#include "server/test/TestServer.h"
#include "comms/StreamData.h"
#include "comms/Data.h"
#include "adapters/test/TestAdapter.h"
#include "adapters/test/TestStreamAdapter.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"

namespace pelican {

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
    TestAdapter serviceAdapter;
    TestStreamAdapter streamAdapter;
    TestServer server;
    QString stream1("stream1");
    QString service1("service1");
    QString version1("version1");
    DataRequirements req;
    req.addStreamData(stream1);
    QByteArray data1("data1");
    QByteArray data2("data2");
    unsigned int port = server.port();

    // common client configuration
    Config config;
    config.setFromString(
            "<testconfig>"
            "   <server host=\"127.0.0.1\"/>"
            "</testconfig>"
            );
    Config::TreeAddress_t address;
    address << Config::NodeId_t("testconfig", "");
    ConfigNode configNode = config.get(address);
    {
        // Use Case:
        // request for Service Data only
        // Expect :
        // throw
        Data sd(service1, version1, data1 );
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
        server.serveServiceData(sd);

        // setup the test
        DataRequirements req;
        req.addServiceData(service1);
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        PelicanServerClient client(configNode, dt);
        client.setPort(port);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db;
        dataHash.insert(service1, &db);
        CPPUNIT_ASSERT_THROW( client.getData(dataHash), QString );
    }
    {
        // Use Case:
        // Single Request for an existing stream dataset
        // Expect:
        // return the required data stream
        StreamData sd(stream1, version1, data1 );
        CPPUNIT_ASSERT_EQUAL( (long)data1.size(), (long)sd.size() );
        server.serveStreamData(sd);

        // setup the test
        QList<DataRequirements> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.addData(lreq);
        dt.setAdapter(stream1, &streamAdapter);
        PelicanServerClient client(configNode, dt);
        client.setPort(port);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db;
        dataHash.insert(stream1, &db);
        client.getData(dataHash);
        CPPUNIT_ASSERT_EQUAL(version1.toStdString(), db.version().toStdString() );
    }
    }
    catch(const QString& e)
    {
        std::cerr << "error: uncaught exception" << e.toStdString();
        throw e;
    }
}

} // namespace pelican
