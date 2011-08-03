#include "ThreadedDataBlobClientTest.h"
#include "ThreadedDataBlobClient.h"


#include "pelican/output/PelicanTCPBlobServer.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ThreadedDataBlobClientTest );
/**
 * @details Constructs a ThreadedDataBlobClientTest object.
 */
ThreadedDataBlobClientTest::ThreadedDataBlobClientTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the ThreadedDataBlobClientTest object.
 */
ThreadedDataBlobClientTest::~ThreadedDataBlobClientTest()
{
}

void ThreadedDataBlobClientTest::setUp()
{
}

void ThreadedDataBlobClientTest::tearDown()
{
}

void ThreadedDataBlobClientTest::test_streamInfo()
{
}

void ThreadedDataBlobClientTest::test_subscribe()
{
}

ThreadedDataBlobClient* ThreadedDataBlobClientTest::_client( PelicanTCPBlobServer* server,
                                                             const QString& xml )
{
    QString conf = xml;
    if( xml == "" )
        conf = QString("<connection host=\"127.0.0.1\" />");
    ConfigNode config(conf);
    ThreadedDataBlobClient* client = new ThreadedDataBlobClient(config);
    client->setPort( server->serverPort() );
    client->setHost("127.0.0.1");
    return client;
}

} // namespace pelican
