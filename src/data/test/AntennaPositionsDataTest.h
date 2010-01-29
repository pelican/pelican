#ifndef ANTENNAPOSITIONSDATATEST_H
#define ANTENNAPOSITIONSDATATEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file AntennaPositionsData.h
 */

/**
 * @class AntennaPositionsData
 *  
 * @brief
 * Unit testing class for the antenna positions data blob.
 * 
 * @details
 * Performs unit tests on the antenna positions data blob object
 * using the CppUnit framework.
 */

namespace pelican {

class AntennaPositionsDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AntennaPositionsDataTest );
        CPPUNIT_TEST( test_accessorMethods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        ///Test accessor methods for visibility positions class
        void test_accessorMethods();

    public:
        AntennaPositionsDataTest();
        ~AntennaPositionsDataTest();

    private:
};

} // namespace pelican

#endif // ANTENNAPOSITIONSDATATEST_H
