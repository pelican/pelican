#ifndef CONFIGTEST_H
#define CONFIGTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ConfigTest.h
 */

/**
 * @class ConfigTest
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class ConfigTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ConfigTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        ConfigTest(  );
        ~ConfigTest();

    private:
};

} // namespace pelican
#endif // CONFIGTEST_H 
