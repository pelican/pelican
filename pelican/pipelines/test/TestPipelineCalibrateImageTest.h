#ifndef TESTPIPELINECALIBRATEIMAGETEST_H
#define TESTPIPELINECALIBRATEIMAGETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TestPipelineCalibrateImageTest.h
 */

class QCoreApplication;

namespace pelican {

/**
 * @class TestPipelineCalibrateImageTest
 *
 * @brief
 *
 * @details
 *
 */

class TestPipelineCalibrateImageTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TestPipelineCalibrateImageTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        TestPipelineCalibrateImageTest();
        ~TestPipelineCalibrateImageTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican
#endif // TESTPIPELINECALIBRATEIMAGETEST_H
