#ifndef DATARECEIVERTEST_H
#define DATARECEIVERTEST_H

/**
 * @file DataReceiverTest.h
 */

#include <QtCore/QString>
#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class DataReceiverTest
 *
 * @brief
 * Unit test for the DataReceiver Class
 *
 * @details
 */

class DataReceiverTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataReceiverTest );
        CPPUNIT_TEST( test_listen );
        CPPUNIT_TEST( test_listen_udpChunker );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_listen();
        void test_listen_udpChunker();

    public:
        DataReceiverTest(  );
        ~DataReceiverTest();
};

} // namespace pelican
#endif // DATARECEIVERTEST_H
