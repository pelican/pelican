#ifndef DATARECEIVERTEST_H
#define DATARECEIVERTEST_H

#include <QString>
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataReceiverTest.h
 */

namespace pelican {

class TestChunker;
class DataManager;

/**
 * @class DataReceiverTest
 *
 * @brief
 *   Unit test for the DataReceiver Class
 * @details
 *
 */

class DataReceiverTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataReceiverTest );
        CPPUNIT_TEST( test_listen );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_listen();

    public:
        DataReceiverTest(  );
        ~DataReceiverTest();

    private:
        TestChunker* _testChunker;
        DataManager* _dm;
        QString _testHost;
        quint16 _testPort;
};

} // namespace pelican
#endif // DATARECEIVERTEST_H
