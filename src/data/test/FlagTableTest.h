#ifndef FLAGTABLETEST_H_
#define FLAGTABLETEST_H_

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file FlagTableTest.h
 */

/**
 * @class FlagTableTest
 *  
 * @brief
 * Unit testing class for the flag data blob.
 * 
 * @details
 * Performs unit tests on the flag data blob object
 * using the CppUnit framework.
 */
namespace pelican {

class FlagTableTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( FlagTableTest );
        CPPUNIT_TEST( test_accessorMethodsIndexed );
        CPPUNIT_TEST( test_accessorMethodsLinear );
        CPPUNIT_TEST( test_emptyBlob );
        CPPUNIT_TEST( test_flag );
        CPPUNIT_TEST( test_resize );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        /// Test indexed accessor methods for 2D flag data blob.
        void test_accessorMethodsIndexed();

        /// Test linear accessor methods for 2D flag data blob.
        void test_accessorMethodsLinear();

        /// Test trying to access an empty blob.
        void test_emptyBlob();

        /// Test writing flag table entries.
        void test_flag();

        /// Test trying to resize empty and non-empty blobs.
        void test_resize();

    public:
        FlagTableTest();
        ~FlagTableTest();

    private:
};

} // namespace pelican

#endif // FLAGTABLETEST_H_
