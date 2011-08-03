#include "pelican/server/test/ServiceDataBufferTest.h"
#include "pelican/server/ServiceDataBuffer.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/LockableServiceData.h"
#include <QtCore/QCoreApplication>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ServiceDataBufferTest );
// class ServiceDataBufferTest 
ServiceDataBufferTest::ServiceDataBufferTest()
    : CppUnit::TestFixture()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

ServiceDataBufferTest::~ServiceDataBufferTest()
{
    delete _app;
}

void ServiceDataBufferTest::setUp()
{
}

void ServiceDataBufferTest::tearDown()
{
}

void ServiceDataBufferTest::test_getWritable()
{
    {
        // Use case:
        // Call getWritable twice without releasing the new object.
        // expect second call to return an invalid object.
        ServiceDataBuffer buffer("test");
        WritableData data1 = buffer.getWritable(1);
        WritableData data2 = buffer.getWritable(1);
        CPPUNIT_ASSERT(data1.isValid());
        CPPUNIT_ASSERT(!data2.isValid());
    }
    
    {
        // Use case:
        // On deleting the WritableData expect it to become current.
        // This should also release the lock allowing a second call
        // to getWritable() to return a valid object
        ServiceDataBuffer buffer("test");
        {
            LockableServiceData* data = 0;
            data = static_cast<LockableServiceData*>(buffer.getWritable(1).data());
            CPPUNIT_ASSERT( data != 0 );
            CPPUNIT_ASSERT( data->isValid() );
        }

        _app->processEvents(); // needed to connect signals and slots

        LockedData currentData("test");
        buffer.getCurrent(currentData);
        CPPUNIT_ASSERT(currentData.isValid());

        // Check that the lock is now released allowing a second getWritable()
        // to return valid data
        WritableData wd = buffer.getWritable(1);
        CPPUNIT_ASSERT( wd.isValid() );
    }
}

void ServiceDataBufferTest::test_retiredData()
{
    {
        // Use Case:
        // dataRetired called on current
        // Expect the data to remain current and not be retired
        AbstractLockableData* d1 = 0;
        ServiceDataBuffer buffer("test");
        {
            WritableData data1 = buffer.getWritable(1);
            CPPUNIT_ASSERT(data1.isValid());
            d1 = data1.data();
            // becomes active when it goes out of scope here
        }
        LockedData locker("test",0);
        buffer.getCurrent(locker);
        CPPUNIT_ASSERT( d1 == locker.object() );
        buffer.deactivateData(static_cast<LockableServiceData*>(d1) );
        LockedData locker2("test",0);
        buffer.getCurrent(locker2);
        CPPUNIT_ASSERT( d1 == locker2.object() );
        CPPUNIT_ASSERT( ! buffer._expiredData.contains(static_cast<LockableServiceData*>(d1) ) );
    }
    {
        // Use Case:
        // dataRetired called on non-current
        // Expect the data to remain available, yet also become available for reuse
        AbstractLockableData* d1 = 0;
        AbstractLockableData* d2 = 0;
        ServiceDataBuffer buffer("test");
        {
            WritableData data1 = buffer.getWritable(1);
            CPPUNIT_ASSERT(data1.isValid());
            d1 = data1.data();
            // becomes active when it goes out of scope here
        }
        {
            WritableData data2 = buffer.getWritable(1);
            CPPUNIT_ASSERT(data2.isValid());
            d2 = data2.data();
            // becomes active current when it goes out of scope here
        }
        LockedData locker("test",0);
        buffer.getCurrent(locker);
        CPPUNIT_ASSERT( d2 == locker.object() );
        buffer.deactivateData(static_cast<LockableServiceData*>(d1) );
        CPPUNIT_ASSERT( buffer._expiredData.contains( static_cast<LockableServiceData*>(d1) ) );
    }
}

} // namespace pelican.
