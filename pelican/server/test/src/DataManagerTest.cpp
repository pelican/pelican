#include "server/test/DataManagerTest.h"
#include "server/DataManager.h"
#include "server/WritableData.h"
#include "server/LockedData.h"
#include "server/LockableStreamData.h"
#include "utility/Config.h"

#include <unistd.h>
#include <iostream>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataManagerTest );

DataManagerTest::DataManagerTest() : CppUnit::TestFixture()
{
}

DataManagerTest::~DataManagerTest()
{
}

void DataManagerTest::test_getWritable()
{
    // Use Case:
    //   Call get Writable with an unknown type
    // Expect:
    //   Return an invalid Writable
    Config config;
    DataManager d(&config);
    WritableData data = d.getWritableData("anything", 100);
    CPPUNIT_ASSERT_EQUAL(false, data.isValid());
}


void DataManagerTest::test_bufferQueryAPI()
{
    // ======== Create configuration for a number of data buffers ==============
    Config config;
    QString bufferConfig = "<buffers>";
    QString type1 = "DataType1";
    size_t type1_maxSize = 10000;
    size_t type1_maxChunkSize = 100;
    bufferConfig += QString(
            "   <%1>"
            "       <buffer maxSize=\"%2\" maxChunkSize=\"%3\" />"
            "   </%1>"
    ).arg(type1).arg(type1_maxSize).arg(type1_maxChunkSize);;
    QString type2 = "DataType2";
    size_t type2_maxSize = 15000;
    size_t type2_maxChunkSize = 200;
    bufferConfig += QString(
            "   <%1>"
            "       <buffer maxSize=\"%2\" maxChunkSize=\"%3\" />"
            "   </%1>"
    ).arg(type2).arg(type2_maxSize).arg(type2_maxChunkSize);
    bufferConfig += QString("</buffers>");
    config.setFromString("", bufferConfig);
    //config.summary();

    // ======= Create a DataManager from the XML configuration =================
    DataManager dm(&config);

    // ======= Create a stream buffer from data type 1 =========================
    dm.getStreamBuffer(type1);
    CPPUNIT_ASSERT_EQUAL(1, dm.numBuffers());
    CPPUNIT_ASSERT_EQUAL(true, dm.isStream(type1));
    CPPUNIT_ASSERT_EQUAL(false, dm.isService(type1));
    CPPUNIT_ASSERT_EQUAL(type1_maxSize, dm.maxSize(type1));
    CPPUNIT_ASSERT_EQUAL(type1_maxChunkSize, dm.maxChunkSize(type1));
    CPPUNIT_ASSERT_EQUAL((size_t)0, dm.allocatedSize(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numChunks(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numExpiredChunks(type1));
    CPPUNIT_ASSERT_EQUAL((size_t)0, dm.usedSize(type1));
    CPPUNIT_ASSERT_EQUAL(type1_maxSize, dm.usableSize(type1, type1_maxChunkSize));
    CPPUNIT_ASSERT_EQUAL((size_t)0, dm.usableSize(type1, type1_maxSize+1));

    // Create a stream chunk by asking for writable data.
    {
        WritableData chunk = dm.getWritableData(type1, type1_maxChunkSize);
        CPPUNIT_ASSERT(chunk.isValid());
        CPPUNIT_ASSERT_EQUAL(type1_maxChunkSize, dm.allocatedSize(type1));
        CPPUNIT_ASSERT_EQUAL(1, dm.numChunks(type1));
        CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type1));
    }
    CPPUNIT_ASSERT_EQUAL(1, dm.numActiveChunks(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numExpiredChunks(type1));
    CPPUNIT_ASSERT_EQUAL((size_t)type1_maxChunkSize, dm.usedSize(type1));
    CPPUNIT_ASSERT_EQUAL(type1_maxSize-type1_maxChunkSize,
            dm.usableSize(type1, type1_maxChunkSize));

    // ======= Serve the chunk =================================================
    // (pull the next stream data chunk from the buffer and mark it as served)
    {
        LockedData chunk = dm.getNext(type1);
        CPPUNIT_ASSERT(chunk.isValid());
        static_cast<LockableStreamData*>(chunk.object())->served() = true;
    }
    CPPUNIT_ASSERT_EQUAL(1, dm.numChunks(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type1));
    CPPUNIT_ASSERT_EQUAL(1, dm.numExpiredChunks(type1));

    // ======= Create a service buffer from data type 2 ========================
    dm.getServiceBuffer(type2);
    CPPUNIT_ASSERT_EQUAL(2, dm.numBuffers());
    CPPUNIT_ASSERT_EQUAL(false, dm.isStream(type2));
    CPPUNIT_ASSERT_EQUAL(true, dm.isService(type2));
    CPPUNIT_ASSERT_EQUAL(type2_maxSize, dm.maxSize(type2));
    CPPUNIT_ASSERT_EQUAL(type2_maxChunkSize, dm.maxChunkSize(type2));
    CPPUNIT_ASSERT_EQUAL((size_t)0, dm.allocatedSize(type2));
    CPPUNIT_ASSERT_EQUAL(0, dm.numChunks(type2));
    CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type2));
    CPPUNIT_ASSERT_EQUAL(0, dm.numExpiredChunks(type2));
    // Create a chunk by asking for writable data.
    {
        WritableData chunk = dm.getWritableData(type2, type2_maxChunkSize);
        CPPUNIT_ASSERT(chunk.isValid());
        CPPUNIT_ASSERT_EQUAL(type2_maxChunkSize, dm.allocatedSize(type2));
        // Note: due the a different allocation mechanism for service
        // data chunks (they are allocated via a temporary chunk called
        // _newData) the number of chunks == 0 until the writable data goes
        // out of scope.
        CPPUNIT_ASSERT_EQUAL(0, dm.numChunks(type2));
        CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type2));
    }
    CPPUNIT_ASSERT_EQUAL(1, dm.numChunks(type2));
    CPPUNIT_ASSERT_EQUAL(1, dm.numActiveChunks(type2));
    CPPUNIT_ASSERT_EQUAL(0, dm.numExpiredChunks(type2));

    // ======= Create another stream chunk. ====================================
    // The DataManager should put this in the empty chunk, so the total number
    // of chunks will not increase.
    {
        WritableData chunk = dm.getWritableData(type1, type1_maxChunkSize);
        CPPUNIT_ASSERT(chunk.isValid());
        CPPUNIT_ASSERT_EQUAL(type1_maxChunkSize, dm.allocatedSize(type1));
        CPPUNIT_ASSERT_EQUAL(1, dm.numChunks(type1));
        CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type1));
    }
    CPPUNIT_ASSERT_EQUAL(1, dm.numChunks(type1));
    CPPUNIT_ASSERT_EQUAL(1, dm.numActiveChunks(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numExpiredChunks(type1));

    // ======= Create another stream chunk. ====================================
    {
        WritableData chunk = dm.getWritableData(type1, type1_maxChunkSize);
    }

    // ======= Create another service data chunk ===============================
    {
        WritableData chunk = dm.getWritableData(type2, type2_maxChunkSize);
        CPPUNIT_ASSERT(chunk.isValid());
    }
    CPPUNIT_ASSERT_EQUAL(2, dm.numChunks(type2));
    CPPUNIT_ASSERT_EQUAL(2, dm.numActiveChunks(type2));

    // ======== Serve the stream chunk and associated service chunk. ===========
    {
        DataSpec requiredData;
        requiredData.addStreamData(type1);
        requiredData.addServiceData(type2);
        QList<LockedData> dataList = dm.getDataRequirements(requiredData);
        foreach (LockedData d, dataList) {
            CPPUNIT_ASSERT(d.isValid());
            LockableStreamData* streamData = static_cast<LockableStreamData*>(
                    d.object());
            CPPUNIT_ASSERT_EQUAL(1, streamData->associateDataTypes().size());
            CPPUNIT_ASSERT(streamData->associateDataTypes().contains(type2));
            streamData->served() = true;
        }
    }
    // We have now served one of the stream data chunks
    CPPUNIT_ASSERT_EQUAL(2, dm.numChunks(type1));
    CPPUNIT_ASSERT_EQUAL(1, dm.numActiveChunks(type1));
    CPPUNIT_ASSERT_EQUAL(1, dm.numExpiredChunks(type1));

    // There are still two active service data chunks, one associated with the
    // remaining stream data and one current (new) chunk not yet associated with
    // anything.
    CPPUNIT_ASSERT_EQUAL(2, dm.numChunks(type2));
    CPPUNIT_ASSERT_EQUAL(2, dm.numActiveChunks(type2));
    CPPUNIT_ASSERT_EQUAL(0, dm.numExpiredChunks(type2));

    // ======== Serve the stream chunk and associated service chunk. ===========
    {
        DataSpec requiredData;
        requiredData.addStreamData(type1);
        requiredData.addServiceData(type2);
        QList<LockedData> dataList = dm.getDataRequirements(requiredData);
        foreach (LockedData d, dataList) {
            CPPUNIT_ASSERT(d.isValid());
            LockableStreamData* streamData = static_cast<LockableStreamData*>(
                    d.object());
            CPPUNIT_ASSERT_EQUAL(1, streamData->associateDataTypes().size());
            CPPUNIT_ASSERT(streamData->associateDataTypes().contains(type2));
            streamData->served() = true;
        }
    }
    // We have now served all of the stream data chunks
    CPPUNIT_ASSERT_EQUAL(2, dm.numChunks(type1));
    CPPUNIT_ASSERT_EQUAL(0, dm.numActiveChunks(type1));
    CPPUNIT_ASSERT_EQUAL(2, dm.numExpiredChunks(type1));

    // There is now one active service data chunk, the current (new chunk)
    // not yet associated with any stream data and the expired one for
    // which all stream data has been served.
    CPPUNIT_ASSERT_EQUAL(2, dm.numChunks(type2));
    CPPUNIT_ASSERT_EQUAL(1, dm.numActiveChunks(type2));
    CPPUNIT_ASSERT_EQUAL(1, dm.numExpiredChunks(type2));

    // NOTE: there probably isn't (and shouldn't be) any way to expire the
    // remaining service data chunk as it will be needed if new stream data
    // arrives.

    CPPUNIT_ASSERT_EQUAL(2, dm.numBuffers());
    CPPUNIT_ASSERT_EQUAL(1, dm.numStreamBuffers());
    CPPUNIT_ASSERT_EQUAL(1, dm.numServiceBuffers());
    CPPUNIT_ASSERT_EQUAL(0, dm.numDeactivatedBuffers());

    // Deactivate streams. This will only happen when the buffer is empty.
    dm.deactivateStream(type1);
    dm.deactivateStream(type2); // This call will do nothing as type2 is not
                                // a stream data type.

    CPPUNIT_ASSERT_EQUAL(1, dm.numDeactivatedBuffers());

    // Now the stream buffer is deactivated we can mark it emptied
    // which removes the stream buffer.
    if (dm.numActiveChunks(type1) == 0) {
        // Note: This function should only be called by the stream buffer
        // and when the buffer is empty!
        dm.emptiedBuffer(dm.getStreamBuffer(type1));
    }

    // There are no longer any deactivated streams as the call to
    // emptiedBuffer() removes it completely.
    CPPUNIT_ASSERT_EQUAL(0, dm.numDeactivatedBuffers());
    CPPUNIT_ASSERT_EQUAL(1, dm.numBuffers());
    CPPUNIT_ASSERT_EQUAL(0, dm.numStreamBuffers());
    CPPUNIT_ASSERT_EQUAL(1, dm.numServiceBuffers());
}



} // namespace pelican
