#ifndef IMAGEWRITERFITS_TEST_H
#define IMAGEWRITERFITS_TEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ImageWriterFitsTest.h
 */

namespace pelican {

/**
 * @class ImageWriterFitsTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ImageWriterFitsTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ImageWriterFitsTest );
        CPPUNIT_TEST( test_createModule );
        CPPUNIT_TEST( test_openFile );
        CPPUNIT_TEST_SUITE_END();


    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_createModule();
        void test_openFile();


    public:
        ImageWriterFitsTest();
        ~ImageWriterFitsTest();

    private:
};

} // namespace pelican

#endif // IMAGEWRITERFITS_TEST_H
