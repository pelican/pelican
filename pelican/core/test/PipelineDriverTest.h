#ifndef PIPELINEDRIVERTEST_H
#define PIPELINEDRIVERTEST_H

#include "pelican/utility/Factory.h"
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file PipelineDriverTest.h
 */

class QCoreApplication;

namespace pelican {

class AbstractModule;
class PipelineDriver;
class DataBlob;
class DataClientFactory;

/**
 * @class PipelineDriverTest
 *
 * @brief
 * CppUnit tests for PipelineDriver class.
 *
 * @details
 *
 */
class PipelineDriverTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PipelineDriverTest );
        CPPUNIT_TEST( test_registerPipeline );
        CPPUNIT_TEST( test_registerPipeline_null );
        CPPUNIT_TEST( test_start_noPipelinesRegistered );
        CPPUNIT_TEST( test_start_noPipelinesRun );
        CPPUNIT_TEST( test_start_singlePipelineNoClient );
        CPPUNIT_TEST( test_start_singlePipelineClientReturnsGoodData );
        CPPUNIT_TEST( test_start_singlePipelineClientReturnsWrongData );
        CPPUNIT_TEST( test_start_multiPipelineRunDifferentData );
        CPPUNIT_TEST( test_start_multiPipelineRunOne );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_registerPipeline();
        void test_registerPipeline_null();
        void test_start_noPipelinesRegistered();
        void test_start_noPipelinesRun();
        void test_start_singlePipelineNoClient();
        void test_start_singlePipelineClientReturnsGoodData();
        void test_start_singlePipelineClientReturnsWrongData();
        void test_start_multiPipelineRunDifferentData();
        void test_start_multiPipelineRunOne();

    public:
        PipelineDriverTest(  );
        ~PipelineDriverTest();

    private:
        QCoreApplication *_coreApp;
        PipelineDriver *_pipelineDriver;
        Factory<DataBlob>* _dataBlobFactory;
        Factory<AbstractModule>* _moduleFactory;
        DataClientFactory* _clientFactory;
};

} // namespace pelican

#endif // PIPELINEDRIVERTEST_H
