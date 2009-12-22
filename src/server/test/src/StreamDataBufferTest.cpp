#include "StreamDataBufferTest.h"
#include "StreamDataBuffer.h"
#include "WritableData.h"
#include "Data.h"
#include "LockedData.h"
#include <QCoreApplication>


#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( StreamDataBufferTest );
// class StreamDataBufferTest 
StreamDataBufferTest::StreamDataBufferTest()
    : CppUnit::TestFixture()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

StreamDataBufferTest::~StreamDataBufferTest()
{
    delete _app;
}

void StreamDataBufferTest::setUp()
{
}

void StreamDataBufferTest::tearDown()
{
}

void StreamDataBufferTest::test_getNext()
{
    {
        // Use case:
        // getNext called on an empty buffer
        // expect to return an invalid LockedData object
        StreamDataBuffer b;
        LockedData data = b.getNext();
        CPPUNIT_ASSERT( ! data.isValid() );
    }
    {
        // Use case:
        // getNext called on an non-empty buffer
        // expect to return an valid LockedData object
        StreamDataBuffer b;
        b.getWritable(100);
        _app->processEvents();
        LockedData data = b.getNext();
        std::cout << data.size() << std::endl;
        CPPUNIT_ASSERT( data.isValid() );
    }
}

} // namespace pelican
