#ifndef DIRECTSTREAMDATACLIENTTEST_H
#define DIRECTSTREAMDATACLIENTTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DirectStreamDataClientTest.h
 */

namespace pelican {

class Config;

/**
 * @class DirectStreamDataClientTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class DirectStreamDataClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DirectStreamDataClientTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        DirectStreamDataClientTest(  );
        ~DirectStreamDataClientTest();

    private:
        Config* _config;
};

} // namespace pelican

#endif // DIRECTSTREAMDATACLIENTTEST_H 
