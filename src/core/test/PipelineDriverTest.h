#ifndef PIPELINEDRIVERTEST_H
#define PIPELINEDRIVERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file PipelineDriverTest.h
 */

/**
 * @class PipelineDriverTest
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class PipelineDriverTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PipelineDriverTest );
        CPPUNIT_TEST( test_registerPipeline );
        CPPUNIT_TEST( test_emptyPipeline );
        CPPUNIT_TEST( test_singlePipeline );
        CPPUNIT_TEST( test_multiPipeline );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_registerPipeline();
        void test_emptyPipeline();
        void test_singlePipeline();
        void test_multiPipeline();

    public:
        PipelineDriverTest(  );
        ~PipelineDriverTest();

    private:
};

} // namespace pelican
#endif // PIPELINEDRIVERTEST_H 
