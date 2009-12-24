#include "ServiceDataBufferTest.h"
#include "ServiceDataBuffer.h"
#include "WritableData.h"
#include "LockedData.h"
#include <QCoreApplication>


#include "utility/memCheck.h"

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
        // Call getWritable twice without releasing the new object
        // expect second call to return an invalid object
        ServiceDataBuffer b;
        WritableData d1 = b.getWritable(1);
        WritableData d2 = b.getWritable(1);
        CPPUNIT_ASSERT( d1.isValid() );
        CPPUNIT_ASSERT( ! d2.isValid() );
    }
    {   
        // Use case:
        // On deleting the WritableData expect it to become current
        // This should also release the lock allowing a second call
        // to getWritable() to return a valid object
        ServiceDataBuffer b;
        {
            Data* d = 0;
            d = b.getWritable(1).data();
            CPPUNIT_ASSERT( d != 0 );
            //CPPUNIT_ASSERT( b.getCurrent().data()[0] == d );
        }
        _app->processEvents();
        WritableData wd = b.getWritable(1);
        CPPUNIT_ASSERT( wd.isValid() );
    }
}

} // namespace pelican
