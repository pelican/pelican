#include "DataReceiverTest.h"
#include "DataReceiver.h"
#include "TestChunker.h"
#include "DataManager.h"
#include "utility/pelicanTimer.h"
#include <QThread>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataReceiverTest );
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
    _testHost = "localhost";
    _testPort = 1444;
    _dm = new DataManager;
//    _testChunker = new TestChunker(_dm);
}

void DataReceiverTest::tearDown()
{
    delete _dm;
//    delete _testChunker;
}

void DataReceiverTest::test_listen()
{
    {
        /* Use Case:
         * Null data Chunker
         * Expect : throw exception
         */
        CPPUNIT_ASSERT_THROW(new DataReceiver(NULL,_dm), QString);
    }

    {
        /* Use Case:
         * Null Socket returned by Chunker
         * Expect : do nothing and return
         */
        TestChunker testChunker(_dm, "test", true, 0, _testHost,_testPort);
        DataReceiver dr(&testChunker,_dm);
        dr.listen();
    }

    {
        /* Use Case:
         * Unspecified host
         * Expect : do nothing and return
         */
        TestChunker testChunker(_dm, "test", false, 0, "",_testPort);
        DataReceiver dr(&testChunker,_dm);
        dr.listen();
    }

    {
        /* Use Case:
         * Unspecified port
         * Expect : do nothing and return
         */
        TestChunker testChunker(_dm, "test", false, 0, "", 0);
        DataReceiver dr(&testChunker,_dm);
        dr.listen();
    }

    {
        /* Use Case:
         * All OK, data sent
         * Expect : to be listening and the Chunker to be receiving data
         *          within its own thread
         */
        TestChunker testChunker(_dm, "test", false, 0, _testHost, _testPort);
        DataReceiver dr(&testChunker,_dm);
        dr.listen();

        // TODO
//        TestSocketServer ts(_testHost, _testPort);
//        CPPUNIT_ASSERT(ts.send("abcd"));
//        _app->processEvents();
//        CPPUNIT_ASSERT_EQUAL(1, _testChunker->nextCalled());
    }
}

} // namespace pelican
