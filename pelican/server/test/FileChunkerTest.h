#ifndef FILECHUNKERTEST_H
#define FILECHUNKERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file FileChunkerTest.h
 */

namespace pelican {

/**
 * @class FileChunkerTest
 * 
 * @brief
 *    unit test for the fileChunker
 * @details
 * 
 */
class FileChunkerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( FileChunkerTest );
        CPPUNIT_TEST( test_startup );
        CPPUNIT_TEST( test_update );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_startup();
        void test_update();

    public:
        /// FileChunkerTest constructor.
        FileChunkerTest();

        /// FileChunkerTest destructor.
        ~FileChunkerTest();

    private:
};

} // namespace pelican

#endif // FILECHUNKERTEST_H 
