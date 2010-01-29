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
 * Unit testing class for the image data blob.
 * 
 * @details
 * Performs unit tests on the image data blob object
 * using the CppUnit framework.
 */

namespace pelican {

class ImageDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImageDataTest );
        CPPUNIT_TEST( test_accessorMethods );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        ///Test accessor methods for image blob
        void test_accessorMethods();

    public:
        ImageDataTest();
        ~ImageDataTest();

    private:
};

} // namespace pelican

#endif // IMAGEDATATEST_H_
