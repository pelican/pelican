#include "pelican/server/test/LockedDataTest.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/LockableServiceData.h"
#include "pelican/server/LockableStreamData.h"


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
        LockedData ld("test");
        CPPUNIT_ASSERT( ! ld.isValid() );
    }
    {
        //Use Case:
        // contains a single data object which is invalid
        // Expect invalid
        LockableServiceData d;
        LockedData ld("test",&d);
        CPPUNIT_ASSERT( ! d.isValid() );
        CPPUNIT_ASSERT( ! ld.isValid() );
    }
}

void LockedDataTest::test_lock()
{
    {
        // Use Case:
        // A valid lockeddata object is copied
        // Expect no unlock until last object is destroyed
        LockableServiceData d("",(void*)1000,100);
        CPPUNIT_ASSERT( d.isValid() ); // sanity check
        CPPUNIT_ASSERT( ! d.isLocked() );
        {
            // create the first lock
            LockedData ld("test",&d);
            CPPUNIT_ASSERT(d.isLocked());
            {
                // create the second lock by copying
                LockedData ld2(ld);
                CPPUNIT_ASSERT(d.isLocked());
            }
            CPPUNIT_ASSERT(d.isLocked());
        }
        CPPUNIT_ASSERT( ! d.isLocked());
    }
    {
        // Use Case:
        // An invalid lockeddata object is copied
        // Expect no unlock until last object is destroyed
        LockableServiceData d("",(void*)0,100);
        CPPUNIT_ASSERT( ! d.isValid() ); // sanity check
        CPPUNIT_ASSERT( ! d.isLocked() );
        {
            // create the first lock
            LockedData ld("test",&d);
            CPPUNIT_ASSERT(d.isLocked());
            {
                // create the second lock by copying
                LockedData ld2(ld);
                CPPUNIT_ASSERT(d.isLocked());
            }
            CPPUNIT_ASSERT(d.isLocked());
        }
        CPPUNIT_ASSERT( ! d.isLocked());
    }
    {
        // Use Case:
        // A valid lockeddata object is copied but the lockeddata
        // are destroyed in the reverse order in which they are created
        // Expect no unlock until last object is destroyed
        LockableServiceData d("",(void*)1000,100);
        CPPUNIT_ASSERT( d.isValid() ); // sanity check
        CPPUNIT_ASSERT( ! d.isLocked() );
        {
            // create the first lock
            LockedData* ld = new LockedData("test",&d);
            CPPUNIT_ASSERT(d.isLocked());
            LockedData* ld2 = new LockedData(*ld);
            CPPUNIT_ASSERT(d.isLocked());
            delete ld;
            CPPUNIT_ASSERT(d.isLocked());
            delete ld2;
            CPPUNIT_ASSERT( ! d.isLocked());
        }
    }
    {
        // Use Case:
        // An invalid lockeddata object is copied but the lockeddata
        // are destroyed in the reverse order in which they are created
        // The data object is a derived type
        // Expect no unlock until last object is destroyed
        LockableStreamData d("",(void*)0,100);
        CPPUNIT_ASSERT( ! d.isValid() ); // sanity check
        CPPUNIT_ASSERT( ! d.isLocked() );
        {
            // create the first lock
            LockedData* ld = new LockedData("test",&d);
            CPPUNIT_ASSERT(d.isLocked());
            LockedData* ld2 = new LockedData(*ld);
            CPPUNIT_ASSERT(d.isLocked());
            delete ld;
            CPPUNIT_ASSERT(d.isLocked());
            delete ld2;
            CPPUNIT_ASSERT( ! d.isLocked());
        }
    }
}

/*
void LockedDataTest::test_size()
{
    {
        //Use Case:
        // contains no data object
        // expect size = 0
        LockedData ld("test");
        CPPUNIT_ASSERT_EQUAL( (size_t)0, ld.size());
    }
    {
        //Use Case:
        // contains invalid data object
        // expect size = 0
        LockableData d("",(void*)0,100);
        LockedData ld("test",&d);
        CPPUNIT_ASSERT_EQUAL( (size_t)0, ld.size());
    }
    {
        //Use Case:
        // contains valid data object
        // expect size = size of data
        LockableData d("",(void*)1000,100);
        LockedData ld("test",&d);
        CPPUNIT_ASSERT_EQUAL( (size_t)100, ld.size());
    }
}*/

} // namespace pelican
