#ifndef PELICANSERVERTEST_H
#define PELICANSERVERTEST_H

/**
 * @file PelicanServerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class PelicanServerTest
 *
 * @brief
 *
 * @details
 */

class PelicanServerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanServerTest );
        CPPUNIT_TEST( test_singleProtocolAcknowledge );
        CPPUNIT_TEST( test_singleProtocolStream );
        CPPUNIT_TEST( test_multiProtocol );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_singleProtocolAcknowledge();
        void test_singleProtocolStream();
        void test_multiProtocol();

    public:
        PelicanServerTest();
        ~PelicanServerTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif // PELICANSERVERTEST_H
