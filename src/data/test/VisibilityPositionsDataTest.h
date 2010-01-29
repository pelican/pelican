#ifndef VISIBILITYPOSITIONSDATATEST_H
#define VISIBILITYPOSITIONSDATATEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file VisibilityPositionsData.h
 */

/**
 * @class VisibilityPositionsData
 *  
 * @brief
 * Unit testing class for the visibility positions data blob.
 * 
 * @details
 * Performs unit tests on the visibility positions data blob object
 * using the CppUnit framework.
 */

namespace pelican {

class VisibilityPositionsDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( VisibilityPositionsDataTest );
        CPPUNIT_TEST( test_accessorMethods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        ///Test accessor methods for visibility positions class
        void test_accessorMethods();

    public:
        VisibilityPositionsDataTest();
        ~VisibilityPositionsDataTest();

    private:
};

} // namespace pelican

#endif // VISIBILITYPOSITIONSDATATEST_H
