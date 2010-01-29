#ifndef IMAGEDATATEST_H_
#define IMAGEDATATEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ImageDataTest.h
 */

/**
 * @class ImageDataTest
 *  
 * @brief
 * Unit testing class for the data requirements object.
 * 
 * @details
 * Performs unit tests on the DataRequirements object
 * using the CppUnit framework.
 */

namespace pelican {

class ImageDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImageDataTest );
        CPPUNIT_TEST( test_assign );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_assign();

    public:
        ImageDataTest();
        ~ImageDataTest();

    private:
};

} // namespace pelican

#endif // IMAGEDATATEST_H_
