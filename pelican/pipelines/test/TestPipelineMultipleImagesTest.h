#ifndef TESTPIPELINEMULTIPLEIMAGESTEST_H
#define TESTPIPELINEMULTIPLEIMAGESTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TestPipelineMultipleImagesTest.h
 */

class QCoreApplication;

namespace pelican {

/**
 * @class TestPipelineMultipleImagesTest
 *
 * @brief
 *
 * @details
 *
 */

class TestPipelineMultipleImagesTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TestPipelineMultipleImagesTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        TestPipelineMultipleImagesTest();
        ~TestPipelineMultipleImagesTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican
#endif // TESTPIPELINEMULTIPLEIMAGESTEST_H
