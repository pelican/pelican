#ifndef PIPELINEAPPLICATIONTEST_H
#define PIPELINEAPPLICATIONTEST_H

/**
 * @file PipelineApplicationTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class PipelineApplicationTest
 *
 * @brief
 * CppUnit tests for PipelineApplication class.
 *
 * @details
 * Tries to create a PipelineApplication:
 * - before a QCoreApplication
 * - after a QCoreApplication
 *
 * Tests parsing of command line arguments:
 * - for setting configuration file
 * - for printing a help message
 *
 * Tests registering one pipeline.
 * Tests registering two pipelines.
 * Tests starting the pipeline driver.
 */
class PipelineApplicationTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(PipelineApplicationTest);
//        CPPUNIT_TEST(test_create_beforeQCoreApplication);
//        CPPUNIT_TEST(test_create_afterQCoreApplication);
//        CPPUNIT_TEST(test_commandLine_config);
//        CPPUNIT_TEST(test_commandLine_help);
//        CPPUNIT_TEST(test_registerPipeline_EmptyPipeline);
//        CPPUNIT_TEST(test_start);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_create_beforeQCoreApplication();
        void test_create_afterQCoreApplication();
        void test_commandLine_config();
        void test_commandLine_help();
        void test_registerPipeline_EmptyPipeline();
        void test_start();

    public:
        PipelineApplicationTest(  );
        ~PipelineApplicationTest();

    private:
};

} // namespace pelican

#endif // PIPELINEAPPLICATIONTEST_H
