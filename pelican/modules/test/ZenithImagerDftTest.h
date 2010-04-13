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
        CPPUNIT_TEST( test_configuration );
        CPPUNIT_TEST( test_inputDataBlobs );
        CPPUNIT_TEST( test_calculateImageCoords );
        CPPUNIT_TEST( test_calculateWeights );
        CPPUNIT_TEST( test_makeImageDft );
        CPPUNIT_TEST( test_run );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_configuration();
        void test_inputDataBlobs();
        void test_calculateImageCoords();
        void test_calculateWeights();
        void test_makeImageDft();
        void test_run();

    public:
        ZenithImagerDftTest();
        ~ZenithImagerDftTest();

    private:
};

} // namespace pelican
#endif // ZENITHIMAGERDFTTEST_H
