#include "StreamDataTest.h"
#include "StreamData.h"
#include "LockedData.h"


#include "utility/memCheck.h"

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
        // Stream Data valid, no associated data
        // expect valid
        StreamData sd((void*)1000,10);
        CPPUNIT_ASSERT( sd.isValid() );
    }
    {
        // Use Case:
        // Stream Data is valid, but associate Data is not
        // expect invalid
        StreamData sd((void*)1000,10);
        Data d; 
        CPPUNIT_ASSERT( ! d.isValid() );
        sd.addAssociatedData( LockedData(&d) );
        CPPUNIT_ASSERT( ! sd.isValid() );
    }
}

} // namespace pelican
