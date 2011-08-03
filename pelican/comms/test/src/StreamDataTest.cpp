#include "pelican/comms/test/StreamDataTest.h"

#include "pelican/comms/StreamData.h"
#include "pelican/comms/DataChunk.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( StreamDataTest );
// class StreamDataTest
StreamDataTest::StreamDataTest()
    : CppUnit::TestFixture()
{
}

StreamDataTest::~StreamDataTest()
{
}

void StreamDataTest::setUp()
{
}

void StreamDataTest::tearDown()
{
}

void StreamDataTest::test_isValid()
{
    {
        // Use Case:
        // Stream Data invalid, no associated data
        // expect invalid
        StreamData sd("",(void*)0,10);
        CPPUNIT_ASSERT( ! sd.isValid() );
    }
    {
        // Use Case:
        // Stream Data valid, no associated data
        // expect valid
        StreamData sd("",(void*)1000,10);
        CPPUNIT_ASSERT( sd.isValid() );
    }
    {
        // Use Case:
        // Stream Data is valid, but associate Data is not
        // expect invalid
        DataChunk d;
        {
            StreamData sd("",(void*)1000,10);
            CPPUNIT_ASSERT( ! d.isValid() );
            CPPUNIT_ASSERT( sd.isValid() );
            boost::shared_ptr<DataChunk> ld( new DataChunk("test", &d) );
            sd.addAssociatedData( ld );
            CPPUNIT_ASSERT( ! sd.isValid() );
        }
    }
}

} // namespace pelican
