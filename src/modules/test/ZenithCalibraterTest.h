#ifndef ZENITHCALIBRATERTEST_H
#define ZENITHCALIBRATERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ZenithCalibraterTest.h
 */

namespace pelican {

/**
 * @class ZenithCalibraterTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ZenithCalibraterTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ZenithCalibraterTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        ZenithCalibraterTest(  );
        ~ZenithCalibraterTest();

    private:
};

} // namespace pelican
#endif // ZENITHCALIBRATERTEST_H 
