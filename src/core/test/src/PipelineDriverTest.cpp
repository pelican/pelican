#include <QCoreApplication>
#include "ModuleFactory.h"
#include "DataClientFactory.h"
#include "core/DataTypes.h"
#include "core/PipelineDriver.h"
#include "core/test/PipelineDriverTest.h"
#include "core/test/TestPipeline.h"
#include "core/test/TestDataClient.h"
#include "data/DataRequirements.h"
#include "data/DataBlobFactory.h"

#include <iostream>

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
 * Tests the registerPipeline() method with a null pointer.
 *
 * An exception of type QString should be thrown.
 */
void PipelineDriverTest::test_registerPipeline_null()
{
    CPPUNIT_ASSERT_THROW(_pipelineDriver->registerPipeline(0), QString);
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
    _pipelineDriver->setIgnoreEmptyHash(false);

    // No data client set, so no data will be returned.
    CPPUNIT_ASSERT_THROW(_pipelineDriver->start(), QString);
}

/**
 * @details
 * Tests the start() method, running a single registered pipeline requiring
 * no remote data, with no data client.
 *
 * Expect the pipeline's run() method to be called repeatedly.
 */
void PipelineDriverTest::test_start_singlePipelineNoClient()
{
    int num = 10;
    TestPipeline *pipeline = new TestPipeline(num);
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline));
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
    CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
}

/**
 * @details
 * Tests the start() method, running a single registered pipeline requiring
 * some remote data, with a client that returns the required data.
 *
 * Expect the pipeline's run() method to be called repeatedly.
 */
void PipelineDriverTest::test_start_singlePipelineClientReturnsGoodData()
{
    // Create the pipeline.
    int num = 10;
    DataRequirements req;
    req.addStreamData("StreamData");
    TestPipeline *pipeline = new TestPipeline(req, num);
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline));
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());

    // Create the data client.
    ConfigNode config;
    DataTypes types;
    types.addData(req);
    TestDataClient client(config, types);
    _pipelineDriver->_dataClient = &client;

    // Start the pipeline driver.
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
    CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
}

/**
 * @details
 * Tests the start() method, running a single registered pipeline requiring
 * some remote data, with a client that returns different data.
 *
 * Expect a throw, since no pipelines are run.
 */
void PipelineDriverTest::test_start_singlePipelineClientReturnsWrongData()
{
    // Create the pipeline.
    int num = 10;
    DataRequirements pipelineReq;
    pipelineReq.addStreamData("StreamData");
    TestPipeline *pipeline = new TestPipeline(pipelineReq, num);
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline));
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());

    // Create the data client.
    ConfigNode config;
    DataTypes types;
    DataRequirements clientTypes;
    clientTypes.addStreamData("OtherStreamData");
    types.addData(clientTypes);
    TestDataClient client(config, types);
    _pipelineDriver->_dataClient = &client;

    // Start the pipeline driver.
    CPPUNIT_ASSERT_THROW(_pipelineDriver->start(), QString);
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());
    CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
}

/**
 * @details
 * Tests the start() method, running two registered pipelines requiring
 * different data. The client returns both sets of data.
 *
 * Expect each pipeline's run() method to be called repeatedly.
 */
void PipelineDriverTest::test_start_multiPipelineRunDifferentData()
{
    // Create both pipelines.
    int num = 10;
    DataRequirements pipelineReq1, pipelineReq2;
    QString type1 = "StreamData1", type2 = "StreamData2";
    pipelineReq1.addStreamData(type1);
    pipelineReq2.addStreamData(type2);
    TestPipeline *pipeline1 = new TestPipeline(pipelineReq1, num);
    TestPipeline *pipeline2 = new TestPipeline(pipelineReq2, num);
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline1));
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline2));
    CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());
    CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());

    // Create the data client.
    ConfigNode config;
    DataTypes types;
    DataRequirements clientTypes;
    clientTypes.addStreamData(type1); // Add both types of required data.
    clientTypes.addStreamData(type2); // Add both types of required data.
    types.addData(clientTypes);
    TestDataClient client(config, types);
    _pipelineDriver->_dataClient = &client;

    // Start the pipeline driver.
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
    CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
    CPPUNIT_ASSERT_EQUAL(num, pipeline2->count());
    CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());
}

/**
 * @details
 * Tests the start() method on two registered pipelines requiring different
 * data, when the client returns only one type of data.
 *
 * Expect one pipeline's run() method to be called repeatedly.
 */
void PipelineDriverTest::test_start_multiPipelineRunOne()
{
    // Create both pipelines.
    int num = 10;
    DataRequirements pipelineReq1, pipelineReq2;
    QString type1 = "StreamData1", type2 = "StreamData2";
    pipelineReq1.addStreamData(type1);
    pipelineReq2.addStreamData(type2);
    TestPipeline *pipeline1 = new TestPipeline(pipelineReq1, num);
    TestPipeline *pipeline2 = new TestPipeline(pipelineReq2, num);
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline1));
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(pipeline2));
    CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());
    CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());

    // Create the data client.
    ConfigNode config;
    DataTypes types;
    DataRequirements clientTypes;
    clientTypes.addStreamData(type1); // Add one type of required data.
    types.addData(clientTypes);
    TestDataClient client(config, types);
    _pipelineDriver->_dataClient = &client;

    // Start the pipeline driver.
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
    CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
    CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());
    CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());
}

} // namespace pelican
