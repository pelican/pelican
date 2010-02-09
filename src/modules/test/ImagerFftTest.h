#ifndef IMAGERFFT_TEST_H
#define IMAGERFFT_TEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ImagerFftTest.h
 */

namespace pelican {

/**
 * @class ImagerFftTest
 *  
 * @brief
 * 
 * @details
 */

class ImagerFftTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImagerFftTest );
        CPPUNIT_TEST( test_createModule );
        CPPUNIT_TEST_SUITE_END();


    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createModule();

    public:
        ImagerFftTest();
        ~ImagerFftTest();

    private:
};

} // namespace pelican

#endif // IMAGERFFT_TEST_H
