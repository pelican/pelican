#ifndef VISIBILITYPOSITIONSTEST_H
#define VISIBILITYPOSITIONSTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file VisibilityPositionsTest.h
 */

/**
 * @class VisibilityPositionsTest
 *  
 * @brief
 * Unit testing class for the visibility positions data blob.
 * 
 * @details
 * Performs unit tests on the visibility positions data blob object
 * using the CppUnit framework.
 */

namespace pelican {

class VisibilityPositionsTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( VisibilityPositionsTest );
        CPPUNIT_TEST( test_accessorMethods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        ///Test accessor methods for visibility positions class
        void test_accessorMethods();

    public:
        VisibilityPositionsTest();
        ~VisibilityPositionsTest();

    private:
};

} // namespace pelican

#endif // VISIBILITYPOSITIONSDATATEST_H
