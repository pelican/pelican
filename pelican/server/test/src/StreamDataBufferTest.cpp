#include "pelican/server/test/StreamDataBufferTest.h"

#include "pelican/server/DataManager.h"
#include "pelican/comms/StreamData.h"
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/utility/Config.h"

#include <QtCore/QCoreApplication>

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
    Config config;
    _dataManager = new DataManager(&config);
}

void StreamDataBufferTest::tearDown()
{
    delete _dataManager;
}

void StreamDataBufferTest::test_getNext()
{
    {
        // Use case:
        // getNext called on an empty buffer
        // expect to return an invalid LockedData object
        StreamDataBuffer b("test");
        b.setDataManager(_dataManager);
        LockedData data("test");
        b.getNext(data);
        CPPUNIT_ASSERT( ! data.isValid() );
    }
    {
    // Use case:
    // getNext called on an non-empty buffer
    // expect to return an valid LockedData object
    StreamDataBuffer b("test");
    b.setDataManager(_dataManager);
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

void StreamDataBufferTest::test_getWritable()
{
    QByteArray data1("data1");
    {
        // Use case:
        // getWritable() called with no service data
        // Expect a valid object with no associate data.
        StreamDataBuffer buffer("test");
        buffer.setDataManager(_dataManager);
        WritableData data = buffer.getWritable(data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        data.write(data1.data(),data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(0, static_cast<LockableStreamData*>(data.data())->associateData().size() );
    }

    // Setup data manager with a service data buffer for remaining test cases
    ServiceDataBuffer* serveBuffer = new ServiceDataBuffer("test");
    QString service1("Service1");
    _dataManager->setServiceDataBuffer(service1,serveBuffer);
    {
        // Use case:
        // getWritable() called with service data supported, but no data
        // expect valid object with no associate data
        StreamDataBuffer b("test");
        b.setDataManager(_dataManager);
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
        StreamDataBuffer b("test");
        b.setDataManager(_dataManager);
        WritableData data = b.getWritable(data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        data.write(data1.data(),data1.size());
        CPPUNIT_ASSERT( data.isValid() );
        CPPUNIT_ASSERT_EQUAL(1, static_cast<LockableStreamData*>(data.data())->associateData().size() );
    }
}


void StreamDataBufferTest::test_getWritableStreams()
{
    std::cout << "#############################################" << std::endl;
    // Use case:
    // Multiple calls to getWritable() simulating filling of a stream buffer.
    // Expect: unique data pointers to be returned.
    {
        size_t dataSize = 8;
        StreamDataBuffer buffer("test");
        buffer.setDataManager(_dataManager);
        {
            WritableData dataChunk = buffer.getWritable(dataSize);
            void* dataPtr =  dataChunk.data()->data()->ptr();
            double value = 1;
            dataChunk.write(&value, 8, 0);
            std::cout << "[1] dataPtr = " << dataPtr << std::endl;
        }
        CPPUNIT_ASSERT_EQUAL(1, buffer._serveQueue.size());
        {
            WritableData dataChunk = buffer.getWritable(dataSize);
            void* dataPtr =  dataChunk.data()->data()->ptr();
            double value = 2;
            dataChunk.write(&value, 8, 0);
            std::cout << "[2] dataPtr = " << dataPtr << std::endl;
        }
        CPPUNIT_ASSERT_EQUAL(2, buffer._serveQueue.size());
        {
            WritableData dataChunk = buffer.getWritable(dataSize);
            void* dataPtr =  dataChunk.data()->data()->ptr();
            double value = 3;
            dataChunk.write(&value, 8, 0);
            std::cout << "[3] dataPtr = " << dataPtr << std::endl;
        }

        CPPUNIT_ASSERT_EQUAL(3, buffer._serveQueue.size());
        CPPUNIT_ASSERT_EQUAL(0, buffer._emptyQueue.size());
        {
            LockedData data("test");
            buffer.getNext(data);
            static_cast<LockableStreamData*>(data.object())->served() = true;
        }
        CPPUNIT_ASSERT_EQUAL(2, buffer._serveQueue.size());
        CPPUNIT_ASSERT_EQUAL(1, buffer._emptyQueue.size());
        {
            LockedData data("test");
            buffer.getNext(data);
            static_cast<LockableStreamData*>(data.object())->served() = true;
        }
        CPPUNIT_ASSERT_EQUAL(1, buffer._serveQueue.size());
        CPPUNIT_ASSERT_EQUAL(2, buffer._emptyQueue.size());

        {
            LockedData data("test");
            buffer.getNext(data);
            static_cast<LockableStreamData*>(data.object())->served() = true;
        }
        CPPUNIT_ASSERT_EQUAL(0, buffer._serveQueue.size());
        CPPUNIT_ASSERT_EQUAL(3, buffer._emptyQueue.size());
        {
            WritableData dataChunk = buffer.getWritable(dataSize);
            void* dataPtr =  dataChunk.data()->data()->ptr();
            double value = 4;
            dataChunk.write(&value, 8, 0);
            std::cout << "[4] dataPtr = " << dataPtr << std::endl;
        }
        CPPUNIT_ASSERT_EQUAL(1, buffer._serveQueue.size());
        CPPUNIT_ASSERT_EQUAL(2, buffer._emptyQueue.size());

    }
    std::cout << "#############################################" << std::endl;
}

} // namespace pelican
