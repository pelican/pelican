#ifndef TESTPIPELINERAWIMAGETEST_H
#define TESTPIPELINERAWIMAGETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TestPipelineRawImageTest.h
 */

class QCoreApplication;

namespace pelican {

/**
 * @class TestPipelineRawImageTest
 *
 * @brief
 *
 * @details
 *
 */

class TestPipelineRawImageTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TestPipelineRawImageTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        TestPipelineRawImageTest();
        ~TestPipelineRawImageTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican

#endif // TESTPIPELINERAWIMAGETEST_H
