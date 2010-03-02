#include "DataTest.h"
#include "Data.h"


#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataTest );
// class DataTest 
DataTest::DataTest()
    : CppUnit::TestFixture()
{
}

DataTest::~DataTest()
{
}

void DataTest::setUp()
{
}

void DataTest::tearDown()
{
}

void DataTest::test_valid()
{
    {
        // Use Case:
        //  size = 0;
        //  expect invalid
       Data d("",(void*)100,0);
       CPPUNIT_ASSERT( ! d.isValid() );
    }
    {
        // Use Case:
        //  data = 0;
        //  expect invalid
       Data d("",0,100);
       CPPUNIT_ASSERT( ! d.isValid() );
    }
    {
        // Use Case:
        //  data = non NULL and size > 0
        //  expect valid
       Data d("",(void*)100,100);
       CPPUNIT_ASSERT( d.isValid() );
    }
}

} // namespace pelican
