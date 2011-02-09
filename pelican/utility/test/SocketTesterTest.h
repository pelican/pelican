#ifndef SOCKETTESTERTEST_H
#define SOCKETTESTERTEST_H

/**
 * @file SocketTesterTest.h
 */

class QCoreApplication;
#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_utility
 *
 * @class SocketTesterTest
 *
 * @brief
 * Unit test for the socket tester
 *
 * @details
 */

class SocketTesterTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( SocketTesterTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        SocketTesterTest();
        ~SocketTesterTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif // SOCKETTESTERTEST_H
