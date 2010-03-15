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
    int ac;
    _app = new QCoreApplication(ac, NULL);
    _testHost = "localhost";
    _testPort = 1444;
    _dm = new DataManager;
    _testChunker = new TestChunker(_dm);
}

void DataReceiverTest::tearDown()
{
    delete _app;
    delete _dm;
    delete _testChunker;
}

void DataReceiverTest::test_listen()
{
//    std::cout << "DataReceiverTest::test_listenA" << std::endl;
//    {
//        /* Use Case:
//         * Null data Chunker
//         * Expect : throw exception
//         */
//        CPPUNIT_ASSERT_THROW(new DataReceiver(NULL,_dm), QString);
//    }
//    std::cout << "DataReceiverTest::test_listenB" << std::endl;
//
//    {
//        /* Use Case:
//         * Null Socket returned by Chunker
//         * Expect : do nothing and return
//         */
//        TestChunker testChunker(_dm, "test", true);
//        DataReceiver dr(&testChunker,_dm);
//        dr.listen(_testHost,_testPort);
//    }
//    std::cout << "DataReceiverTest::test_listenC" << std::endl;
//    {
//        /* Use Case:
//         * Unspecified host
//         * Expect : do nothing and return
//         */
//        DataReceiver dr(_testChunker,_dm);
//        dr.listen("",_testPort);
//    }
//    std::cout << "DataReceiverTest::test_listenD" << std::endl;
//    {
//        /* Use Case:
//         * Unspecified port
//         * Expect : do nothing and return
//         */
//        DataReceiver dr(_testChunker,_dm);
//        dr.listen("", 0);
//    }
//    std::cout << "DataReceiverTest::test_listenE" << std::endl;
    {
        /* Use Case:
         * All OK, data sent
         * Expect : to be listening and the Chunker to be receiving data
         *          within its own thread
         */
        std::cout << "---------------\n";
        DataReceiver dr(_testChunker,_dm);
        dr.listen(_testHost, _testPort);


        // TODO
//        TestSocketServer ts(_testHost, _testPort);
//        CPPUNIT_ASSERT(ts.send("abcd"));
//        _app->processEvents();
//        CPPUNIT_ASSERT_EQUAL(1, _testChunker->nextCalled());
    }
//    std::cout << "DataReceiverTest::test_listen Done!" << std::endl;
}

} // namespace pelican
