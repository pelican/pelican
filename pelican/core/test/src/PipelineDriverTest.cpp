#include "pelican/core/test/PipelineDriverTest.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/output/OutputStreamManager.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/core/DataTypes.h"
#include "pelican/core/PipelineDriver.h"
#include "pelican/core/test/TestPipeline.h"
#include "pelican/core/test/TestDataClient.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/test/TestDataBlob.h"

#include <QtCore/QCoreApplication>

#include <iostream>

namespace pelican {

using test::TestPipeline;
using test::TestDataClient;

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
    //int argc = 1;
    //char *argv[] = {(char*)"pelican"};
    //_coreApp = new QCoreApplication(argc, argv);

    // Create the factories.
    _dataBlobFactory = new FactoryGeneric<DataBlob>(false);
    Config config;
    Config::TreeAddress address;

    _moduleFactory = new FactoryConfig<AbstractModule>(0, "", "");

    _clientFactory = new DataClientFactory(0, "", "", 0);

    _osmanager = new OutputStreamManager(0,Config::TreeAddress());

    // Create the pipeline driver.
    _pipelineDriver = new PipelineDriver( _dataBlobFactory, _moduleFactory,
                                          _clientFactory, _osmanager, &config, address);
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
    delete _osmanager;

    // Delete the QCoreApplication.
    //delete _coreApp;
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

    // try a pipeline with an unknown datablob type
    DataRequirements req;
    req.setStreamData("requiredData");
    CPPUNIT_ASSERT_THROW(_pipelineDriver->registerPipeline(new TestPipeline(req)), QString);

    // try a pipeline with an known datablob type
    DataRequirements req2;
    req2.setStreamData("TestDataBlob");
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(new TestPipeline(req2)));
}

void PipelineDriverTest::test_registerSwitcher()
{
    {
        // Use Case:
        // An empty switcher
        // expect: throw a QString
        PipelineSwitcher sw;
        CPPUNIT_ASSERT_THROW(_pipelineDriver->addPipelineSwitcher(sw), QString);
    }
    {
        // Use Case:
        // A switcher with two pipelines requiring no data
        // then kill the first pipeline
        // expect : start processing first pipeline, and switch to the second
        int num = 10;
        PipelineSwitcher sw;
        TestPipeline* p1 = new TestPipeline(num);
        p1->setDeactivation(true);
        CPPUNIT_ASSERT(p1->deactivation());
        TestPipeline* p2 = new TestPipeline(num);
        sw.addPipeline(p1);
        CPPUNIT_ASSERT(p1->deactivation());
        CPPUNIT_ASSERT(!p2->deactivation());
        sw.addPipeline(p2);
        _pipelineDriver->addPipelineSwitcher(sw);
        CPPUNIT_ASSERT_EQUAL(0, p1->count());
        CPPUNIT_ASSERT(p1->deactivation());
        CPPUNIT_ASSERT_EQUAL(false, p2->deactivation());
        CPPUNIT_ASSERT_EQUAL(0, p2->count());
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, p1->count());
        CPPUNIT_ASSERT_EQUAL(num, p2->count());
    }
}

void PipelineDriverTest::test_registerSwitcherData()
{
    try {
    DataRequirements req;
    req.setStreamData("TestDataBlob");

    // Create the data client.
    ConfigNode config;
    DataTypes types;
    types.addData(req);
    TestDataClient client(config, types);
    _pipelineDriver->_dataClient = &client;

    {
        // Use Case:
        // A switcher with two pipelines requiring the same data
        // then kill the first pipeline
        // expect : start processing first pipeline, and switch to the second
        int num = 10;
        PipelineSwitcher sw;
        TestPipeline* p1 = new TestPipeline(req, num);
        p1->setDeactivation(true);
        TestPipeline* p2 = new TestPipeline(req, num);
        sw.addPipeline(p1);
        sw.addPipeline(p2);
        _pipelineDriver->addPipelineSwitcher(sw);
        CPPUNIT_ASSERT(p1->deactivation());
        CPPUNIT_ASSERT(!p2->deactivation());
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, p1->count());
        CPPUNIT_ASSERT_EQUAL(num, p2->count());
    }
    }
    catch( QString e ) {
        CPPUNIT_FAIL("Caught QString(\"" + e.toStdString() + "\")");
    }
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
    req.setStreamData("TestDataBlob");
    _pipelineDriver->registerPipeline(new TestPipeline(req));

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
    try {
        // Create the pipeline.
        int num = 10;
        DataRequirements req;
        req.addStreamData("FloatData");
        TestPipeline *pipeline = new TestPipeline(req, num);
        _pipelineDriver->registerPipeline(pipeline);
        CPPUNIT_ASSERT_EQUAL(0, pipeline->count());

        // Create the data client.
        ConfigNode config;
        DataTypes types;
        types.addData(req);
        TestDataClient client(config, types);
        _pipelineDriver->_dataClient = &client;

        // Start the pipeline driver.
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
        CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
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
    try {
        // Create the pipeline.
        int num = 10;
        DataRequirements pipelineReq;
        pipelineReq.addStreamData("FloatData");
        TestPipeline *pipeline = new TestPipeline(pipelineReq, num);
        _pipelineDriver->registerPipeline(pipeline);
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
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
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
    try {
        // Create both pipelines.
        int num = 10;
        DataRequirements pipelineReq1, pipelineReq2;
        QString type1 = "FloatData", type2 = "DoubleData";
        pipelineReq1.addStreamData(type1);
        pipelineReq2.addStreamData(type2);
        TestPipeline *pipeline1 = new TestPipeline(pipelineReq1, num);
        TestPipeline *pipeline2 = new TestPipeline(pipelineReq2, num);
        _pipelineDriver->registerPipeline(pipeline1);
        _pipelineDriver->registerPipeline(pipeline2);
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
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
        CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
        CPPUNIT_ASSERT_EQUAL(num, pipeline2->count());
        CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
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
    try {
        // Create both pipelines.
        int num = 10;
        DataRequirements pipelineReq1, pipelineReq2;
        QString type1 = "FloatData", type2 = "DoubleData";
        pipelineReq1.addStreamData(type1);
        pipelineReq2.addStreamData(type2);
        TestPipeline *pipeline1 = new TestPipeline(pipelineReq1, num);
        TestPipeline *pipeline2 = new TestPipeline(pipelineReq2, num);
        _pipelineDriver->registerPipeline(pipeline1);
        _pipelineDriver->registerPipeline(pipeline2);
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
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
        CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
        CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());
        CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());

        // ensure history size is just one
        CPPUNIT_ASSERT_EQUAL(1, pipeline1->streamHistory(type1).size() );

    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}
/**
 * @details
 * Test that data is stored in the correct buffers
 */
void PipelineDriverTest::test_start_pipelineWithHistory()
{
    try {
        { // Use Case:
          // One data stream with history
          // Expect:
          // Ability to access all data up to the history limit
            int num = 10;
            int history=5;
            DataRequirements pipelineReq;
            QString type1 = "FloatData";
            pipelineReq.addStreamData(type1);
            TestPipeline *pipeline1 = new TestPipeline(pipelineReq, num);
            pipeline1->setHistory(type1, history);
            _pipelineDriver->registerPipeline(pipeline1);
            CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());

            // Create the data client.
            ConfigNode config;
            DataTypes types;
            DataRequirements clientTypes;
            clientTypes.addStreamData(type1); // Add one type of required data.
            types.addData(clientTypes);
            TestDataClient client(config, types);
            _pipelineDriver->_dataClient = &client;

            // Start the pipeline driver.
            _pipelineDriver->start();
            CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
            CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());

            // check the history
            const QList<DataBlob* > h = pipeline1->streamHistory(type1);
            CPPUNIT_ASSERT_EQUAL( history, h.size() );
            CPPUNIT_ASSERT( h[0] != h[1]  ); // ensure different blobs
        }
    }
    catch(const QString& e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
