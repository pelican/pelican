#ifndef TIMESTREAMDATATEST_H
#define TIMESTREAMDATATEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TimeStreamDataTest.h
 */

/**
 * @class TimeStreamDataTest
 *
 * @brief
 * Unit testing class for the time stream data blob.
 *
 * @details
 * Performs unit tests on the time stream data blob object
 * using the CppUnit framework.
 */

namespace pelican {

class TimeStreamDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TimeStreamDataTest );
        CPPUNIT_TEST( test_accessorMethods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        ///Test accessor methods for visibility positions class
        void test_accessorMethods();

    public:
        TimeStreamDataTest();
        ~TimeStreamDataTest();

    private:
};

} // namespace pelican

#endif // TIMESTREAMDATATEST_H
