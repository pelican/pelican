#include "DataReceiverTest.h"
#include "DataReceiver.h"
#include "TestChunker.h"
#include "DataManager.h"

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
    _testChunker = new TestChunker(_dm);
}

void DataReceiverTest::tearDown()
{
    delete _dm;
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
        TestChunker testChunker(_dm, "test", true);
        DataReceiver dr(&testChunker,_dm);
        dr.listen(_testHost,_testPort);
    }
    {
        /* Use Case:
         * Unspecified host
         * Expect : do nothing and return
         */
        DataReceiver dr(_testChunker,_dm);
        dr.listen("",_testPort);
    }
    {
        /* Use Case:
         * Unspecified port
         * Expect : do nothing and return
         */
        DataReceiver dr(_testChunker,_dm);
        dr.listen("", 0);
    }
    {
        /* Use Case:
         * All OK, data sent
         * Expect : to be listening and the Chunker to be receiving data
         *          within its own thread
         */
        DataReceiver dr(_testChunker,_dm);
        dr.listen(_testHost, _testPort);
        /* TODO
        TestSocketServer ts(_testHost, _testPort);
        CPPUNIT_ASSERT(ts.send("abcd"));
        _app->processEvents();
        CPPUNIT_ASSERT_EQUAL(1, _testChunker->nextCalled());
        */
    }
}

} // namespace pelican
