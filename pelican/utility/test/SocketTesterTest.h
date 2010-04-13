#ifndef SOCKETTESTERTEST_H
#define SOCKETTESTERTEST_H

class QCoreApplication;
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file SocketTesterTest.h
 */

namespace pelican {

/**
 * @class SocketTesterTest
 *  
 * @brief
 *  Unit tets for the socket tester
 * @details
 * 
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
        SocketTesterTest(  );
        ~SocketTesterTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif // SOCKETTESTERTEST_H 
