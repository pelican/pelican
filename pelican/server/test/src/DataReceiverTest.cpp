#include "pelican/server/test/DataReceiverTest.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/server/test/TestChunker.h"
#include "pelican/server/test/TestUdpChunker.h"
#include "pelican/emulator/EmulatorDriver.h"
#include "pelican/emulator/test/RealUdpEmulator.h"
#include "pelican/server/DataManager.h"
#include "pelican/utility/pelicanTimer.h"
#include "pelican/utility/Config.h"

#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

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
    std::cout <<" ========================================== " << std::endl;
    try {
        // Create Data Manager
        Config config;
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
                "    <packet size=\"512\" interval=\"1000\" initialValue=\"0.1\"/>"
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

        // Create the data receiver.
        DataReceiver dr(&chunker);
        dr.listen();

        // Must call processEvents() for the data to emit the
        // unlockedWrite() signal.
        sleep(1);
        QCoreApplication::processEvents();

        // Test read data
        LockedData d = dataManager.getNext("VisibilityData");
        // std::cout << "Is valid: " << d.isValid() << std::endl;
        char* dataPtr = (char *)(reinterpret_cast<AbstractLockableData*>(d.object())->data()->data() );
        double value = *reinterpret_cast<double*>(dataPtr);
        std::cout << "Value : " << value << std::endl;
    }

    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
