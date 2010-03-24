#ifndef TESTPIPELINESERVERTEST_H
#define TESTPIPELINESERVERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TestPipelineServerTest.h
 */

namespace pelican {

/**
 * @class TestPipelineServerTest
 *
 * @brief
 *
 * @details
 *
 */
class TestPipelineServerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TestPipelineServerTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    private:
        // Create XML configuration
        void _createConfig();

    public:
        TestPipelineServerTest();
        ~TestPipelineServerTest();

    private:
};

} // namespace pelican

#endif // TESTPIPELINESERVERTEST_H
