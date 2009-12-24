#include "DataManager.h"
#include "StreamData.h"
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
    _dataManager = new DataManager;
}

void StreamDataBufferTest::tearDown()
{
    delete _dataManager;
}

void StreamDataBufferTest::test_getWritable()
{
    {
        // Use case:
        // getWritable() called with no service data
        // expect valid object with no associate data
        StreamDataBuffer b(_dataManager);
        WritableData data = b.getWritable(1);
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(0, static_cast<StreamData*>(data.data())->associateData().size() );
    }
    // setup data manager with a service data buffer for remaining test cases
    ServiceDataBuffer serveBuffer;
    QString service1("Service1");
    _dataManager->serviceDataBuffer(service1,&serveBuffer);
    {
        // Use case:
        // getWritable() called with service data supported, but no data
        // expect valid object with no associate data
        StreamDataBuffer b(_dataManager);
        WritableData data = b.getWritable(1);
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(0, static_cast<StreamData*>(data.data())->associateData().size() );
    }
    // inject some data into the service buffer for remaining tests
    serveBuffer.getWritable(1);
    _app->processEvents();
    {
        // Use case:
        // getWritable() called with service data supported, with data
        // expect valid object with associate data
        StreamDataBuffer b(_dataManager);
        WritableData data = b.getWritable(1);
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(1, static_cast<StreamData*>(data.data())->associateData().size() );
    }
}

void StreamDataBufferTest::test_getNext()
{
    {
        // Use case:
        // getNext called on an empty buffer
        // expect to return an invalid LockedData object
        StreamDataBuffer b(_dataManager);
        LockedData data = b.getNext();
        CPPUNIT_ASSERT( ! data.isValid() );
    }
    {
        // Use case:
        // getNext called on an non-empty buffer
        // expect to return an valid LockedData object
        StreamDataBuffer b(_dataManager);
        b.getWritable(100);
        _app->processEvents();
        LockedData data = b.getNext();
        CPPUNIT_ASSERT( data.isValid() );
        // Use Case:
        // Make a second call to getNext() whilst data exists but locked
        // Expect to return an invalid object
        LockedData data2 = b.getNext();
        CPPUNIT_ASSERT( ! data2.isValid() );
    }
}

} // namespace pelican
