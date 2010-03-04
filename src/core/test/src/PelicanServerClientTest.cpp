#include <QList>
#include <QString>
#include <QDomElement>
#include <QCoreApplication>
#include <QTcpSocket>
#include "PelicanServerClientTest.h"
#include "PelicanServerClient.h"
#include "adapters/AdapterFactory.h"
#include "data/DataRequirements.h"
#include "adapters/test/TestDataBlob.h"
#include "adapters/test/TestAdapterFactory.h"
#include "adapters/test/TestAdapter.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"
#include "utility/SocketTester.h"
#include "comms/StreamDataResponse.h"
#include "comms/ServiceDataResponse.h"


#include "utility/memCheck.h"

namespace pelican {

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
     int argc = 1;
     char *argv[] = {(char*)"pelican"};
     _coreApp = new QCoreApplication(argc, argv);
}

void PelicanServerClientTest::tearDown()
{
    delete _coreApp;
}

void PelicanServerClientTest::test_getData()
{
    Config config;
    ConfigNode configNode;
    AdapterFactory factory(&config);
    {
        // Use Case:
        //    Empty data requirements
        // Expect: throw with a suitable complaint

        // setup the test
        QList<DataRequirements> lreq;
        CPPUNIT_ASSERT_THROW( new PelicanServerClient(configNode, &factory, lreq), QString );
    }
    QString stream1("stream1");
    DataRequirements req;
    req.addStreamData(stream1);
    {
        // Use Case:
        //    data requirements are set, empty hash passed
        // Expect: silently do nothing

        QList<DataRequirements> lreq;
        lreq.append(req);
        PelicanServerClient client(configNode, &factory, lreq);

        QHash<QString, DataBlob*> dataHash;
        CPPUNIT_ASSERT_NO_THROW(client.getData(dataHash));
    }
    /*
    {
        // Use Case:
        //    Data Requirements do not match DataBlobs
        // Expect: 
        //     throw with a suitable message

        // setup the test
        QList<DataRequirements> lreq;
        lreq.append(req);
        PelicanServerClient client(configNode, &factory, lreq);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db;
        dataHash.insert("data2", &db);

        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString);
    }
    {
        // Use Case:
        // Single Request for an existing single dataset
        //

        // setup the test
        QList<DataRequirements> lreq;
        lreq.append(req);
        PelicanServerClient client(configNode, &factory, lreq);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db;
        dataHash.insert(stream1, &db);
        client.getData(dataHash);
    }
    */
}

void PelicanServerClientTest::test_response()
{
    ConfigNode configNode;
    TestAdapterFactory factory;
    QString stream1("stream1");
    QString version1("v1");
    DataRequirements req;
    req.addStreamData(stream1);
    QByteArray data1("data1");
    {
        // Use Case
        // StreamData response (single stream, no service data)
        boost::shared_ptr<ServerResponse> res( new StreamDataResponse );
        static_cast<StreamDataResponse*>(res.get())->setStreamData( new StreamData(stream1,version1) );

        QList<DataRequirements> lreq;
        lreq.append(req);
        PelicanServerClient client(configNode, &factory, lreq);
        TestDataBlob db;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(stream1, &db);

        SocketTester st;
        QTcpSocket& sock = st.send(data1);
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), dataHash[stream1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data1.data()) , std::string( static_cast<TestDataBlob*>(dataHash[stream1])->data() ) );
    }
    QByteArray data2("data2");
    {
        // Use Case
        // ServiceData response
    }
}

} // namespace pelican
