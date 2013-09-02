#include "server/test/DataReceiverTest.h"
#include "server/DataReceiver.h"
#include "server/test/TestChunker.h"
#include "server/test/TestUdpChunker.h"
#include "server/LockedData.h"
#include "server/LockableStreamData.h"
#include "emulator/EmulatorDriver.h"
#include "emulator/test/RealUdpEmulator.h"
#include "server/DataManager.h"
#include "utility/pelicanTimer.h"
#include "utility/Config.h"


#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

#include <cfloat>
#include <unistd.h>
#include <iostream>

namespace pelican {

using test::TestChunker;
using test::TestUdpChunker;
using test::RealUdpEmulator;

CPPUNIT_TEST_SUITE_REGISTRATION(DataReceiverTest);

// class DataReceiverTest
DataReceiverTest::DataReceiverTest() : CppUnit::TestFixture()
{
}

DataReceiverTest::~DataReceiverTest()
{
}

void DataReceiverTest::test_listen()
{
    // Set up the test.
    QString testHost = "localhost";
    quint16 testPort = 1444;
    Config config;
    DataManager dataManager(&config);

    {
        /* Use Case:
         * Null data Chunker
         * Expect : throw exception
         */
        CPPUNIT_ASSERT_THROW(new DataReceiver(NULL), QString);
    }

    {
        /* Use Case:
         * Null Socket returned by Chunker
         * Expect : do nothing and return
         */
        TestChunker testChunker("test", true, 0, testHost,testPort);
        testChunker.setDataManager(&dataManager);
        DataReceiver dr(&testChunker);
        dr.listen();
    }

    {
        /* Use Case:
         * Unspecified host
         * Expect : do nothing and return
         */
        TestChunker testChunker("test", false, 0, "",testPort);
        testChunker.setDataManager(&dataManager);
        DataReceiver dr(&testChunker);
        dr.listen();
    }

    {
        /* Use Case:
         * Unspecified port
         * Expect : do nothing and return
         */
        TestChunker testChunker("test", false, 0, "", 0);
        testChunker.setDataManager(&dataManager);
        DataReceiver dr(&testChunker);
        dr.listen();
    }

    {
        /* Use Case:
         * All OK, data sent
         * Expect : to be listening and the Chunker to be receiving data
         *          within its own thread
         */
        TestChunker testChunker("test", false, 0, testHost, testPort);
        testChunker.setDataManager(&dataManager);
        DataReceiver dr(&testChunker);
        dr.listen();
    }
}


/*
 * Use case:
 *  Tests the data receiver interface in the server for the case of a UDP
 *  data stream.
 *
 *  This is performed by creating:
 *  - A DataManager to hold a stream data buffer.
 *  - A UDP chunker and associated emulator to feed data into the DataReciever.
 *
 * The data receiver drives the chunker to receive data from the stream
 * (I/O device) written to by the emulator.
 *
 * Received data is checked by querying the data manager.
 */
void DataReceiverTest::test_listen_udpChunker()
{
    using namespace std;
    try {
        // DataManager
        //   The DataManager is the main class for accessing data in the server.
        //
        // Data buffer configuration (the DataManager uses the <buffers>
        // section of the XML. Respected attributes of the <buffer>:
        //   maxSize      = maximum buffer size in bytes (default = 10240)
        //   maxChunkSize = maximum chunk size in bytes (default = maxSize)
        QString configString =
                "<buffers>"
                "   <VisibilityData>"
                "       <buffer maxSize=\"5120\" maxChunkSize=\"512\"/>"
                "   </VisibilityData>"
                "</buffers>";
        Config config;
        config.setFromString(configString);
        DataManager dataManager(&config, "pipeline");
        // Set up a stream buffer for the specified data type, configured
        // by the XML configuration above.
        StreamDataBuffer* streamBuffer = dataManager.getStreamBuffer("VisibilityData");

        // Create and set up chunker.
        QString chunkerNodeString = ""
                "<TestUdpChunker>"
                "   <connection host=\"127.0.0.1\" port=\"2002\"/>"
                "   <data type=\"VisibilityData\" chunkSize=\"512\"/>"
                "</TestUdpChunker>";
        ConfigNode chunkerNode(chunkerNodeString);
        TestUdpChunker chunker(chunkerNode);
        chunker.setDataManager(&dataManager);

        // Create the DataReceiver (QThread managing the chunker, the event
        // loop in this thread calls next() on the chunker)
        DataReceiver dr(&chunker);
        // Starts the event loop in the DataReciever thread. This starts the
        // chunker, creating the device in the chunker and then starting
        // the event loop which processes incoming data.
        dr.listen();

        // Start the emulator to send UDP packets to the chunker.
        ConfigNode emulatorConfig(""
                "<RealUdpEmulator>"
                "    <connection host=\"127.0.0.1\" port=\"2002\"/>"
                "    <packet number=\"3\" size=\"512\" interval=\"1000\" initialValue=\"0.1\"/>"
                "</RealUdpEmulator>"
        );
        EmulatorDriver emulator(new RealUdpEmulator(emulatorConfig));

        // Must call processEvents() for the data to emit the unlockedWrite()
        // signal.
        usleep(4000); // Sleep needs to long enough for the 3 packets to arrive.
        QCoreApplication::processEvents();

        // Test read of the data from the DataManager.
        // This function asks the DataManager for the next visibility data
        // chunk and checks its value.
        for (int p = 0; p < 3; ++p)
        {
            CPPUNIT_ASSERT_EQUAL(3-p, streamBuffer->numberOfActiveChunks());

            LockedData d = dataManager.getNext("VisibilityData");
            CPPUNIT_ASSERT(d.isValid());
            char* data = (char*)(reinterpret_cast<AbstractLockableData*>(
                    d.object())->data()->data());
            double* values = reinterpret_cast<double*>(data);
            // This value should be p+0.1
            CPPUNIT_ASSERT_DOUBLES_EQUAL(p+0.1, values[0], DBL_EPSILON);

            // Set the data to served to access the next entry in the buffer.
            static_cast<LockableStreamData*>(d.object())->served() = true;
        }
    }
    catch (const QString& e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
