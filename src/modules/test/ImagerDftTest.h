#ifndef IMAGERDFT_TEST_H
#define IMAGERDFT_TEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ImagerDftTest.h
 */

namespace pelican {

/**
 * @class ImagerDftTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ImagerDftTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImagerDftTest );
        CPPUNIT_TEST( test_createModule );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createModule();

    public:
        ImagerDftTest();
        ~ImagerDftTest();

    private:
};

} // namespace pelican

#endif // IMAGERDFT_TEST_H
