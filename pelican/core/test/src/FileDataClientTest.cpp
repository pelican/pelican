#include "FileDataClientTest.h"
#include "FileDataClient.h"

#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"
#include "pelican/core/DataTypes.h"
#include "pelican/core/test/TestStreamAdapter.h"

namespace pelican {
using test::TestStreamAdapter;

CPPUNIT_TEST_SUITE_REGISTRATION( FileDataClientTest );
/**
 * @details Constructs a FileDataClientTest object.
 */
FileDataClientTest::FileDataClientTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the FileDataClientTest object.
 */
FileDataClientTest::~FileDataClientTest()
{
}

void FileDataClientTest::setUp()
{
}

void FileDataClientTest::tearDown()
{
}

void FileDataClientTest::test_factory()
{
    // Use case:
    // Instantiate with a factory
    try {
    Config config;
    config.setFromString(
            "<adapters>"
            "   <file name=\"testfile.xml\"/>"
            "</adapters>"
            );
    Config::TreeAddress address;
    address << Config::NodeId("adapters", "");
    ConfigNode configNode = config.get(address);
    }
    catch( QString& e ) {
        CPPUNIT_FAIL(e.toStdString());
    }

}

void FileDataClientTest::test_method()
{
    try {
    Config config;
    config.setFromString(
            "<testconfig>"
            "   <file name=\"testfile.xml\"/>"
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
        FileDataClient* client = 0;
        CPPUNIT_ASSERT_THROW(client = new FileDataClient(configNode, dt, 0), QString);
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
        FileDataClient client(configNode, dt, 0);

        QHash<QString, DataBlob*> dataHash;
        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString );
    }
    {  // Use Case:
       // Specifiy a single file and associate it with a Stream
       // correct hash is passed
       // Expect:
       // Data in file to be streamed
       QList<DataRequirements> lreq;
        lreq.append(req);
       DataTypes types;
       types.addData(lreq);
       types.setAdapter(stream1,&streamAdapter);
       FileDataClient client(configNode, types, &config);

       QHash<QString, DataBlob*> dataHash;
       DataBlob db("DataBlob");
       dataHash.insert(stream1, &db);
       client.getData(dataHash);
    }
    }
    catch( QString& e ) {
        CPPUNIT_FAIL(e.toStdString());
    }
}

} // namespace pelican
