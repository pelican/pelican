#ifndef FILEDATACLIENTTEST_H
#define FILEDATACLIENTTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file FileDataClientTest.h
 */

namespace pelican {

/**
 * @class FileDataClientTest
 * 
 * @brief
 * 
 * @details
 * 
 */
class FileDataClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( FileDataClientTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST( test_factory );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();
        void test_factory();

    public:
        /// FileDataClientTest constructor.
        FileDataClientTest();

        /// FileDataClientTest destructor.
        ~FileDataClientTest();

    private:
};

} // namespace pelican

#endif // FILEDATACLIENTTEST_H 
