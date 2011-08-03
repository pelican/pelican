#include "pelican/comms/test/DataChunkTest.h"
#include "pelican/comms/DataChunk.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataChunkTest );

// class DataTest
DataChunkTest::DataChunkTest()
: CppUnit::TestFixture()
{
}

DataChunkTest::~DataChunkTest()
{
}

void DataChunkTest::setUp()
{
}

void DataChunkTest::tearDown()
{
}

void DataChunkTest::test_valid()
{
    {
        // Use Case:
        // size = 0;
        // expect invalid
        DataChunk d("",(void*)100,0);
        CPPUNIT_ASSERT( ! d.isValid() );
    }
    {
        // Use Case:
        // data = 0;
        // expect invalid
        DataChunk d("",0,100);
        CPPUNIT_ASSERT( ! d.isValid() );
    }
    {
        // Use Case:
        // data = non NULL and size > 0
        // expect valid
        DataChunk d("",(void*)100,100);
        CPPUNIT_ASSERT( d.isValid() );
    }
}

void DataChunkTest::test_pointer()
{
    QByteArray b;
    b.resize(100);
    DataChunk d("test",b.data(),100);
    CPPUNIT_ASSERT( b.data() == d.ptr() );
}

} // namespace pelican
