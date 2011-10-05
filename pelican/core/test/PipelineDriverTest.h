#ifndef PIPELINEDRIVERTEST_H
#define PIPELINEDRIVERTEST_H

/**
 * @file PipelineDriverTest.h
 */

#include "pelican/data/DataBlob.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/FactoryGeneric.h"

#include <cppunit/extensions/HelperMacros.h>

class QCoreApplication;

namespace pelican {

class PipelineDriver;
class DataClientFactory;
class OutputStreamManager;

/**
 * @ingroup t_core
 *
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
        CPPUNIT_TEST( test_registerSwitcher );
        CPPUNIT_TEST( test_registerSwitcherData );
        CPPUNIT_TEST( test_registerPipeline_null );
        CPPUNIT_TEST( test_start_noPipelinesRegistered );
        CPPUNIT_TEST( test_start_noPipelinesRun );
        CPPUNIT_TEST( test_start_singlePipelineNoClient );
        CPPUNIT_TEST( test_start_singlePipelineClientReturnsGoodData );
        CPPUNIT_TEST( test_start_singlePipelineClientReturnsWrongData );
        CPPUNIT_TEST( test_start_multiPipelineRunDifferentData );
        CPPUNIT_TEST( test_start_multiPipelineRunOne );
        CPPUNIT_TEST( test_start_pipelineWithHistory );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_registerPipeline();
        void test_registerSwitcher();
        void test_registerSwitcherData();
        void test_registerPipeline_null();
        void test_start_noPipelinesRegistered();
        void test_start_noPipelinesRun();
        void test_start_singlePipelineNoClient();
        void test_start_singlePipelineClientReturnsGoodData();
        void test_start_singlePipelineClientReturnsWrongData();
        void test_start_multiPipelineRunDifferentData();
        void test_start_multiPipelineRunOne();
        void test_start_pipelineWithHistory();

    public:
        PipelineDriverTest(  );
        ~PipelineDriverTest();

    private:
        QCoreApplication *_coreApp;
        PipelineDriver *_pipelineDriver;
        FactoryGeneric<DataBlob>* _dataBlobFactory;
        FactoryConfig<AbstractModule>* _moduleFactory;
        DataClientFactory* _clientFactory;
        OutputStreamManager* _osmanager;
};

} // namespace pelican

#endif // PIPELINEDRIVERTEST_H
