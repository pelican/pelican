#include <QCoreApplication>
#include "ModuleFactory.h"
#include "DataClientFactory.h"
#include "core/PipelineDriver.h"
#include "core/test/PipelineDriverTest.h"
#include "core/test/TestPipeline.h"
#include "data/DataRequirements.h"
#include "data/DataBlobFactory.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PipelineDriverTest );

/**
 * @details
 * PipelineDriverTest constructor.
 */
PipelineDriverTest::PipelineDriverTest()
    : CppUnit::TestFixture()
{
    _coreApp = NULL;
    _pipelineDriver = NULL;
    _dataBlobFactory = NULL;
    _moduleFactory = NULL;
    _clientFactory = NULL;
}

/**
 * @details
 * PipelineDriverTest destructor.
 */
PipelineDriverTest::~PipelineDriverTest()
{
}

/**
 * @details
 * Set-up routine executed before each test method.
 */
void PipelineDriverTest::setUp()
{
    // Create the QCoreApplication.
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _coreApp = new QCoreApplication(argc, argv);

    // Create the factories.
    _dataBlobFactory = new DataBlobFactory;
    Config config;
    _moduleFactory = new ModuleFactory(&config);
    Config::TreeAddress_t clientsNode;
    _clientFactory = new DataClientFactory(0, clientsNode, 0);

    // Create the pipeline driver.
    _pipelineDriver = new PipelineDriver(_dataBlobFactory, _moduleFactory,
            _clientFactory);
}

/**
 * @details
 * Clean-up routine executed after each test method.
 */
void PipelineDriverTest::tearDown()
{
    // Delete the pipeline driver.
    delete _pipelineDriver;

    // Delete the factories.
    delete _dataBlobFactory;
    delete _moduleFactory;
    delete _clientFactory;

    // Delete the QCoreApplication.
    delete _coreApp;
}

/**
 * @details
 * Tests the registerPipeline() method with a pipeline that requires no data,
 * and with a pipeline that requires data.
 *
 * No exceptions should be thrown in either case.
 */
void PipelineDriverTest::test_registerPipeline()
{
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(new TestPipeline));

    DataRequirements req;
    req.setStreamData("requiredData");
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(new TestPipeline(req)));
}

/**
 * @details
 * Tests the start() method when no pipelines have been registered.
 *
 * An exception of type QString should be thrown.
 */
void PipelineDriverTest::test_start_noPipelinesRegistered()
{
    CPPUNIT_ASSERT_THROW(_pipelineDriver->start(), QString);
}

/**
 * @details
 * Tests the start() method when the data returned by the client via getData()
 * is not compatible with any registered pipeline.
 *
 * An exception of type QString should be thrown.
 */
void PipelineDriverTest::test_start_noPipelinesRun()
{
    DataRequirements req;
    req.setStreamData("requiredData");
    _pipelineDriver->registerPipeline(new TestPipeline(req));

    // No data client set, so no data will be returned.
    CPPUNIT_ASSERT_THROW(_pipelineDriver->start(), QString);
}

void PipelineDriverTest::test_singlePipeline()
{
    // Use Case:
    // Attempt to run a single registered pipeline.
    // Expect run method to be called with appropriate data on the test pipeline (repeatedly).

//    QString wibble("wibble");
//    TestDataClient client;
//    QSet<QString> set;
//    set.insert(wibble);
//    client.setSubset(set);
//    
/*
    _pipelineDriver->setDataClient("Test");

    DataRequirements req;
    req.setStreamData(wibble);
    TestPipeline *pipeline = new TestPipeline(req);
    pipeline->setPipelineDriver(_pipelineDriver);
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline));
    int num = 10;
    pipeline->setIterations(num);
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
    CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
    */
}

void PipelineDriverTest::test_multiPipeline()
{
    return;
    // Use Case:
    // Attempt to run multiple registered pipelines requiring different data.
    // Expect run method to be called with appropriate data on the test pipelines (repeatedly).

//    QString reqData1("wibble1");
//    QString reqData2("wibble2");
//    TestDataClient client;
//    QSet<QString> set;
//    set.insert(reqData1);
//    set.insert(reqData2);
//    client.setSubset(set);
//    _pipelineDriver->setDataClient(&client);
//
//    DataRequirements req1;
//    DataRequirements req2;
//    req1.setStreamData(reqData1);
//    req2.setStreamData(reqData2);
//    TestPipeline *pipeline1 = new TestPipeline(req1);
//    TestPipeline *pipeline2 = new TestPipeline(req2);
//    pipeline1->setPipelineDriver(_pipelineDriver);
//    pipeline2->setPipelineDriver(_pipelineDriver);
//    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline1));
//    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline2));
//    int num = 10;
//    pipeline1->setIterations(num);
//    pipeline2->setIterations(num);
//    CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());
//    CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());
//
//    // Use Case:
//    // Data returned matches both pipelines.
//    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
//    CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
//    CPPUNIT_ASSERT_EQUAL(num, pipeline2->count());
//    CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
//    CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());
//
//    // Reset pipeline counters.
//    pipeline1->reset();
//    pipeline2->reset();
//
//    // Use Case:
//    // Data returned matches a single pipeline.
//    QSet<QString> monoSet;
//    monoSet.insert(reqData1);
//    client.setSubset(monoSet);
//    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
//    CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
//    CPPUNIT_ASSERT_EQUAL(num, pipeline2->count());
//    CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
//    CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());

}

} // namespace pelican
