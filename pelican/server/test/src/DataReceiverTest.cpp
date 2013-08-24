#include "server/test/DataReceiverTest.h"
#include "server/DataReceiver.h"
#include "server/test/TestChunker.h"
#include "server/test/TestUdpChunker.h"
#include "emulator/EmulatorDriver.h"
#include "emulator/test/RealUdpEmulator.h"
#include "server/DataManager.h"
#include "utility/pelicanTimer.h"
#include "utility/Config.h"

#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

#include <cfloat>

namespace pelican {

using test::TestChunker;
using test::TestUdpChunker;
using test::RealUdpEmulator;

CPPUNIT_TEST_SUITE_REGISTRATION(DataReceiverTest);

// class DataReceiverTest
DataReceiverTest::DataReceiverTest()
    : CppUnit::TestFixture()
{
}

DataReceiverTest::~DataReceiverTest()
{
}

void DataReceiverTest::setUp()
{
}

void DataReceiverTest::tearDown()
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

        // TODO
//        TestSocketServer ts(testHost, testPort);
//        CPPUNIT_ASSERT(ts.send("abcd"));
//        _app->processEvents();
//        CPPUNIT_ASSERT_EQUAL(1, _testChunker->nextCalled());
    }
}

void DataReceiverTest::test_listen_udpChunker()
{
    // FIXME write detailed use case for this test and check that it still
    // works with the modifications to DataReceiver in 1.0.4

//    using namespace std;
//    cout << endl << __PRETTY_FUNCTION__ << endl;
    try {
        // Create Data Manager
        Config config;
        // FIXME are maxSize and maxChunkSize valid? see packet size of the
        // emulator...?
        QString configString =
                "<buffers>"
                "   <VisibilityData>"
                "       <buffer maxSize=\"2000\" maxChunkSize=\"2000\"/>"
                "   </VisibilityData>"
                "</buffers>";
        config.setFromString(configString);
        DataManager dataManager(&config, "pipeline");
        dataManager.getStreamBuffer("VisibilityData");

        // Start the telescope.
        ConfigNode emulatorConfig(""
                "<RealUdpEmulator>"
                "    <connection host=\"127.0.0.1\" port=\"2002\"/>"
                "    <packet number=\"10\" size=\"512\" interval=\"1000\" initialValue=\"0.1\"/>"
                "</RealUdpEmulator>"
                );
        EmulatorDriver emulator(new RealUdpEmulator(emulatorConfig));

        // Create and set up chunker.
        QString chunkerNodeString = ""
                "<TestUdpChunker>"
                "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
                "</TestUdpChunker>";
        ConfigNode chunkerNode(chunkerNodeString);
        TestUdpChunker chunker(chunkerNode);
        chunker.setDataManager(&dataManager);

        // Create the data receiver (this is a QThread managing the chunker?)
        DataReceiver dr(&chunker);
        // listen() starts the event loop in the DataReciever thread.
        dr.listen();

        // Must call processEvents() for the data to emit the unlockedWrite()
        // signal.
        // FIXME which data... ?
        sleep(1);
        QCoreApplication::processEvents();

        // Test read of the data from the DataManager.
        // This function asks the DataManager for the next visibility
        // data chunk and checks/prints its value.
        {
            LockedData d = dataManager.getNext("VisibilityData");
            CPPUNIT_ASSERT_EQUAL(true, d.isValid());
            char* data = (char *)(reinterpret_cast<AbstractLockableData*>(
                    d.object())->data()->data());
            double value = *reinterpret_cast<double*>(data);
            // This value should be 0.1?, the initial value of the emulated packet.
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, value, DBL_EPSILON);
//            cout << "Value : " << value << endl;
        }
    }

    catch (const QString& e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
