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
 * Tests various functions of the basic flagging module.
 * 
 * @details
 * Performs unit tests on the basic flagging module
 * using the CppUnit framework.
 */
class BasicFlaggerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( BasicFlaggerTest );
        CPPUNIT_TEST( test_run_noData );
        CPPUNIT_TEST( test_run_withData );
        CPPUNIT_TEST( test__flagAutocorrelations );
        CPPUNIT_TEST( test__getAutocorrelations );
        CPPUNIT_TEST( test__getMedians );
        CPPUNIT_TEST( test__moveBadAntennas );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_run_noData();
        void test_run_withData();
        void test__flagAutocorrelations();
        void test__getAutocorrelations();
        void test__getMedians();
        void test__moveBadAntennas();

    public:
        BasicFlaggerTest(  );
        ~BasicFlaggerTest();

    private:
        BasicFlagger* _basicFlagger;
};

} // namespace pelican
#endif // BASICFLAGGERTEST_H 
