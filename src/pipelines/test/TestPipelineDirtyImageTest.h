#ifndef TESTPIPELINEDIRTYIMAGETEST_H
#define TESTPIPELINEDIRTYIMAGETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TestPipelineDirtyImageTest.h
 */

class QCoreApplication;

namespace pelican {

/**
 * @class TestPipelineDirtyImageTest
 *
 * @brief
 *
 * @details
 *
 */

class TestPipelineDirtyImageTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TestPipelineDirtyImageTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        TestPipelineDirtyImageTest();
        ~TestPipelineDirtyImageTest();

    private:
        QCoreApplication *_app;
};

} // namespace pelican
#endif // TESTPIPELINEDIRTYIMAGETEST_H
