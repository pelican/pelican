#ifndef ANTENNAGAINSTEST_H_
#define ANTENNAGAINSTEST_H_

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file AntennaGainsTest.h
 */

/**
 * @class AntennaGainsTest
 *  
 * @brief
 * Unit testing class for the antenna gains data blob.
 * 
 * @details
 * Performs unit tests on the antenna gains data blob object
 * using the CppUnit framework.
 */
namespace pelican {

class AntennaGainsTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( AntennaGainsTest );
        CPPUNIT_TEST( test_accessorMethodsIndexed );
        CPPUNIT_TEST( test_accessorMethodsLinear );
        CPPUNIT_TEST( test_emptyBlob );
        CPPUNIT_TEST( test_resize );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        /// Test indexed accessor methods for gains data blob.
        void test_accessorMethodsIndexed();

        /// Test linear accessor methods for gains data blob.
        void test_accessorMethodsLinear();

        /// Test trying to access an empty blob.
        void test_emptyBlob();

        /// Test trying to resize empty and non-empty blobs.
        void test_resize();

    public:
        AntennaGainsTest();
        ~AntennaGainsTest();

    private:
};

} // namespace pelican

#endif // ANTENNAGAINSTEST_H_
