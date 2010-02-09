#ifndef IMAGERFFT_TEST_H
#define IMAGERFFT_TEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ImagerFFTTest.h
 */

namespace pelican {

/**
 * @class ImagerFFTTest
 *  
 * @brief
 * 
 * @details
 */

class ImagerFFTTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImagerFFTTest );
        CPPUNIT_TEST( test_createModule );
        CPPUNIT_TEST_SUITE_END();


    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createModule();

    public:
        ImagerFFTTest();
        ~ImagerFFTTest();

    private:
};

} // namespace pelican

#endif // IMAGERFFT_TEST_H
