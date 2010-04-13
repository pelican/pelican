#ifndef ZENITHMODELVISIBILITIESTEST_H
#define ZENITHMODELVISIBILITIESTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ZenithModelVisibilitiesTest.h
 */

namespace pelican {

/**
 * @class ZenithModelVisibilitiesTest
 *  
 * @brief
 * 
 * @details
 */

class ZenithModelVisibilitiesTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ZenithModelVisibilitiesTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        ZenithModelVisibilitiesTest(  );
        ~ZenithModelVisibilitiesTest();

    private:
};

} // namespace pelican
#endif // ZENITHMODELVISIBILITIESTEST_H 
