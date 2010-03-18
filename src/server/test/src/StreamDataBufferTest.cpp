#include "DataManager.h"
#include "comms/StreamData.h"
#include "StreamDataBufferTest.h"
#include "StreamDataBuffer.h"
#include "WritableData.h"
#include "LockedData.h"
#include "LockableStreamData.h"
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
    QByteArray data1("data1");
    {
        // Use case:
        // getWritable() called with no service data
        // expect valid object with no associate data
        StreamDataBuffer b("test",_dataManager);
        WritableData data = b.getWritable(data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        data.write(data1.data(),data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(0, static_cast<LockableStreamData*>(data.data())->associateData().size() );
    }
    // setup data manager with a service data buffer for remaining test cases
    ServiceDataBuffer* serveBuffer = new ServiceDataBuffer("test");
    QString service1("Service1");
    _dataManager->serviceDataBuffer(service1,serveBuffer);
    {
        // Use case:
        // getWritable() called with service data supported, but no data
        // expect valid object with no associate data
        StreamDataBuffer b("test",_dataManager);
        WritableData data = b.getWritable(data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        data.write(data1.data(),data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(0, static_cast<LockableStreamData*>(data.data())->associateData().size() );
    }
    // inject some data into the service buffer for remaining tests
    serveBuffer->getWritable(1);
    _app->processEvents();
    {
        // Use case:
        // getWritable() called with service data supported, with data
        // expect valid object with associate data
        StreamDataBuffer b("test",_dataManager);
        WritableData data = b.getWritable(data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        data.write(data1.data(),data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(1, static_cast<LockableStreamData*>(data.data())->associateData().size() );
    }
}

void StreamDataBufferTest::test_getNext()
{
    {
        // Use case:
        // getNext called on an empty buffer
        // expect to return an invalid LockedData object
        StreamDataBuffer b("test",_dataManager);
        LockedData data("test");
        b.getNext(data);
        CPPUNIT_ASSERT( ! data.isValid() );
    }
    {
        // Use case:
        // getNext called on an non-empty buffer
        // expect to return an valid LockedData object
        StreamDataBuffer b("test",_dataManager);
        b.getWritable(100);
        _app->processEvents();
        LockedData data("test");
        b.getNext(data);
        CPPUNIT_ASSERT( data.isValid() );
        // Use Case:
        // Make a second call to getNext() whilst data exists but locked
        // Expect to return an invalid object
        LockedData data2("test2");
        b.getNext(data2);
        CPPUNIT_ASSERT( ! data2.isValid() );
    }
}

} // namespace pelican
