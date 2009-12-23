#include "LockedDataTest.h"
#include "LockedData.h"
#include "Data.h"


#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( LockedDataTest );
// class LockedDataTest 
LockedDataTest::LockedDataTest()
    : CppUnit::TestFixture()
{
}

LockedDataTest::~LockedDataTest()
{
}

void LockedDataTest::setUp()
{
}

void LockedDataTest::tearDown()
{
}

void LockedDataTest::test_isValid()
{
    {
        //Use Case:
        // contains no data object
        // Expect invalid
        LockedData ld;
        CPPUNIT_ASSERT( ! ld.isValid() );
    }
    {
        //Use Case:
        // contains a single data object which is invalid
        // Expect invalid
        Data d;
        LockedData ld(&d);
        CPPUNIT_ASSERT( ! d.isValid() );
        CPPUNIT_ASSERT( ! ld.isValid() );
    }
    {
        //Use Case:
        // contains a two data objects one of which is invalid
        // Expect invalid
        Data d(0,0);
        Data d2((void*)100,100);
        LockedData ld(&d);
        CPPUNIT_ASSERT( ! d.isValid() );
        ld.addData(&d2);
        CPPUNIT_ASSERT( d2.isValid() );
        CPPUNIT_ASSERT( ! ld.isValid() );
    }
    {
        //Use Case:
        // contains  two data objects both of which are valid
        // Expect valid
        Data d((void*)200,100);
        Data d2((void*)100,100);
        LockedData ld(&d);
        CPPUNIT_ASSERT( d.isValid() );
        ld.addData(&d2);
        CPPUNIT_ASSERT( d2.isValid() );
        CPPUNIT_ASSERT( ld.isValid() );
    }
}

void LockedDataTest::test_size()
{
    {
        //Use Case:
        // contains no data object
        // expect size = 0
        LockedData ld;
        CPPUNIT_ASSERT_EQUAL( (size_t)0, ld.size());
    }
    {
        //Use Case:
        // contains invalid data object
        // expect size = 0
        Data d((void*)0,100);
        LockedData ld(&d);
        CPPUNIT_ASSERT_EQUAL( (size_t)0, ld.size());
    }
    {
        //Use Case:
        // contains valid data object
        // expect size = size of data
        Data d((void*)1000,100);
        LockedData ld(&d);
        CPPUNIT_ASSERT_EQUAL( (size_t)100, ld.size());
    }
    {
        //Use Case:
        // contains multiple valid data objects
        // expect size = sum of data sizes
        Data d((void*)1000,100);
        Data d2((void*)2000,33);
        LockedData ld(&d);
        ld.addData(&d2);
        CPPUNIT_ASSERT_EQUAL( (size_t)133, ld.size());
    }
}
} // namespace pelican
