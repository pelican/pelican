#ifndef VISIBILITYDATATEST_H_
#define VISIBILITYDATATEST_H_

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file VisibilityDataTest.h
 */

/**
 * @class VisibilityDataTest
 *  
 * @brief
 * Unit testing class for the visibility data blob.
 * 
 * @details
 * Performs unit tests on the visibility data blob object
 * using the CppUnit framework.
 */
namespace pelican {

class VisibilityDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( VisibilityDataTest );
        CPPUNIT_TEST( test_accessorMethods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        /// Test accessor methods for visibility data blob.
        void test_accessorMethods();

    public:
        VisibilityDataTest();
        ~VisibilityDataTest();

    private:
};

} // namespace pelican

#endif // VISIBILITYDATATEST_H_
