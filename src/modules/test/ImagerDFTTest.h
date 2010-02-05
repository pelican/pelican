#ifndef IMAGERDFT_TEST_H
#define IMAGERDFT_TEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ImagerDFTTest.h
 */

namespace pelican {

/**
 * @class ImagerDFTTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ImagerDFTTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImagerDFTTest );
        CPPUNIT_TEST( test_createModule );
        CPPUNIT_TEST_SUITE_END();


    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createModule();

    public:
        ImagerDFTTest();
        ~ImagerDFTTest();

    private:
};

} // namespace pelican

#endif // IMAGERDFT_TEST_H
