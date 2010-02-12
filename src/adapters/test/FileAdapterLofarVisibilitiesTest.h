#ifndef FILEADAPTERLOFARVISIBILITIESTEST_H
#define FILEADAPTERLOFARVISIBILITIESTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file FileAdapterLofarVisibilitiesTest.h
 */

namespace pelican {

/**
 * @class FileAdapterLofarVisibilitiesTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class FileAdapterLofarVisibilitiesTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( FileAdapterLofarVisibilitiesTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        FileAdapterLofarVisibilitiesTest(  );
        ~FileAdapterLofarVisibilitiesTest();

    private:
};

} // namespace pelican
#endif // FILEADAPTERLOFARVISIBILITIESTEST_H 
