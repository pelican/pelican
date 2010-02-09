#ifndef ZENITHIMAGERDFTTEST_H
#define ZENITHIMAGERDFTTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ZenithImagerDftTest.h
 */

namespace pelican {

/**
 * @class ZenithImagerDftTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ZenithImagerDftTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ZenithImagerDftTest );
        CPPUNIT_TEST( test_assign );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_assign();

    public:
        ZenithImagerDftTest();
        ~ZenithImagerDftTest();

    private:
};

} // namespace pelican
#endif // ZENITHIMAGERDFTTEST_H 
