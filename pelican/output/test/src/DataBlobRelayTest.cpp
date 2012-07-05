#include "DataBlobRelayTest.h"
#include "DataBlobRelay.h"
#include "TestDataBlobClient.h"
#include "TestOutputStreamer.h"
#include "Stream.h"
#include "pelican/utility/Config.h"
#include "pelican/data/test/TestDataBlob.h"
#include <boost/shared_ptr.hpp>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobRelayTest );
/**
 *@details DataBlobRelayTest 
 */
DataBlobRelayTest::DataBlobRelayTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataBlobRelayTest::~DataBlobRelayTest()
{
}

void DataBlobRelayTest::setUp()
{
}

void DataBlobRelayTest::tearDown()
{
}

void DataBlobRelayTest::test_method()
{
     Config config;
     QString streamId("testStream");
     {
         // Use Case: empty config
         // Expect: no issues constructing object
         Config::TreeAddress address;
         DataBlobRelay r( &config, address );

        // add a client
        test::TestDataBlobClient client;
        r.addClient( &client );

        // add a Streamer
        test::TestOutputStreamer testStreamer;
        r.connectToStream( &testStreamer, streamId );

        // check data flow
        boost::shared_ptr<DataBlob> testData( new test::TestDataBlob );
        Stream stream(streamId);
        stream.setData( testData );
        CPPUNIT_ASSERT_EQUAL( (void*)0, (void*)testStreamer.lastReceived( streamId ));
        client.send( stream );
        CPPUNIT_ASSERT_EQUAL( (void*)testData.get(), (void*)testStreamer.lastReceived( streamId ));
     }
}

} // namespace pelican
