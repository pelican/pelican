#ifndef BASICFLAGGERTEST_H
#define BASICFLAGGERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file BasicFlaggerTest.h
 */

namespace pelican {

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
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        BasicFlaggerTest(  );
        ~BasicFlaggerTest();

    private:
};

} // namespace pelican
#endif // BASICFLAGGERTEST_H 
