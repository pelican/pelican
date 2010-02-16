#ifndef MYPIPELINETEST_H
#define MYPIPELINETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file MyPipelineTest.h
 */

namespace pelican {

/**
 * @class MyPipelineTest
 *
 * @brief
 *
 * @details
 *
 */

class MyPipelineTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( MyPipelineTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        MyPipelineTest();
        ~MyPipelineTest();

    private:
};

} // namespace pelican
#endif // MYPIPELINETEST_H
