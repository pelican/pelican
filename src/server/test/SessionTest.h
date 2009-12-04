#ifndef SESSIONTEST_H
#define SESSIONTEST_H

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

/**
 * @file SessionTest.h
 */

namespace pelican {

/**
 * @class SessionTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class SessionTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( SessionTest );
        CPPUNIT_TEST( test_processRequest );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_processRequest();

    public:
        SessionTest(  );
        ~SessionTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif // SESSIONTEST_H 
