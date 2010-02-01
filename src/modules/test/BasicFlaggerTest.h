#ifndef BASICFLAGGERTEST_H
#define BASICFLAGGERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <QDomElement>

/**
 * @file BasicFlaggerTest.h
 */

namespace pelican {

class BasicFlagger;

/**
 * @class BasicFlaggerTest
 *  
 * @brief
 * 
 * @details
 * 
 */
class BasicFlaggerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( BasicFlaggerTest );
        CPPUNIT_TEST( test_run_noData );
        CPPUNIT_TEST( test_run_withData );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_run_noData();
        void test_run_withData();

    public:
        BasicFlaggerTest(  );
        ~BasicFlaggerTest();

    private:
        BasicFlagger* _basicFlagger;
};

} // namespace pelican
#endif // BASICFLAGGERTEST_H 
