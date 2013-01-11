/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/test/PipelineDriverTest.h"
#include "modules/AbstractModule.h"
#include "output/OutputStreamManager.h"
#include "core/DataClientFactory.h"
#include "core/DataTypes.h"
#include "core/PipelineDriver.h"
#include "core/test/TestPipeline.h"
#include "core/test/TestDataClient.h"
#include "data/DataRequirements.h"
#include "data/test/TestDataBlob.h"

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
    _client = NULL; // will be set if required
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
    delete _client;
}

void PipelineDriverTest::test_checkPipelineRequirements()
{
    ConfigNode config;
    QString stream1("stream1");
    QString stream2("stream2");
    {
        // Use case:
        // pipeline has required data only.
        // Client has this and other data as stream data
        // Expect:
        // Requirements to be selected and marked as stream data
        DataRequirements req; req.addRequired(stream1);
        DataSpec spec;
        spec.addStreamData(stream1);
        spec.addStreamData(stream2);
        TestDataClient client(config, spec);
        TestPipeline* p1 = new TestPipeline( req );
        _pipelineDriver->_checkPipelineRequirements( p1, &client );
        CPPUNIT_ASSERT( (_pipelineDriver->_dataSpecs[p1].streamData()) == (req.required()) );
        CPPUNIT_ASSERT_EQUAL(0 , _pipelineDriver->_dataSpecs[p1].serviceData().size() );
    }
    {
        // Use case:
        // pipeline has required data only.
        // Client has this as service data other data as stream data
        // Expect:
        // Requirements to be selected and marked as service data
        DataRequirements req; req.addRequired(stream1);
        DataSpec spec;
        spec.addServiceData(stream1);
        spec.addStreamData(stream2);
        TestDataClient client(config, spec);
        TestPipeline* p1 = new TestPipeline( req );
        _pipelineDriver->_checkPipelineRequirements( p1, &client );
        CPPUNIT_ASSERT( (_pipelineDriver->_dataSpecs[p1].serviceData()) == (req.required()) );
        CPPUNIT_ASSERT_EQUAL(0 , _pipelineDriver->_dataSpecs[p1].streamData().size() );
    }
    {
        // Use case:
        // pipeline has required data and optional data.
        // Client has this as service data other data as stream data
        // Expect:
        // Requirements to be selected and marked as stream/service data
        DataRequirements req; req.addRequired(stream1);
        req.addOptional(stream2);
        DataSpec spec;
        spec.addServiceData(stream1);
        spec.addStreamData(stream2);
        TestDataClient client(config, spec);
        TestPipeline* p1 = new TestPipeline( req );
        _pipelineDriver->_checkPipelineRequirements( p1, &client );
        CPPUNIT_ASSERT( (_pipelineDriver->_dataSpecs[p1].serviceData()) == (req.required()) );
        CPPUNIT_ASSERT( (_pipelineDriver->_dataSpecs[p1].streamData()) == (req.optional()) );
    }
    {
        // Use case:
        // pipeline has required data
        // Client does not have this data
        // Expect:
        // throw
        DataRequirements req; req.addRequired(stream1);
        DataSpec spec;
        TestDataClient client(config, spec);
        TestPipeline* p1 = new TestPipeline( req );
        CPPUNIT_ASSERT_THROW(_pipelineDriver->_checkPipelineRequirements( p1, &client ), QString );

    }
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
    /* current implementation will not throw until start!
    DataRequirements req;
    req.addRequired("requiredData");
    CPPUNIT_ASSERT_THROW(_pipelineDriver->registerPipeline(new TestPipeline(req)), QString);
    */

    // try a pipeline with an known datablob type
    DataRequirements req2;
    req2.addRequired("TestDataBlob");
    CPPUNIT_ASSERT_NO_THROW(_pipelineDriver->registerPipeline(new TestPipeline(req2)));
}

void PipelineDriverTest::test_registerSwitcher()
{
    _setTestClient();
    {
        // Use Case:
        // An empty switcher
        // expect: throw a QString
        PipelineSwitcher sw;
        CPPUNIT_ASSERT_THROW(_pipelineDriver->addPipelineSwitcher(sw), QString);
    }
    try
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
    catch( const QString& s ) {
        CPPUNIT_FAIL( s.toStdString() );
    }
}

void PipelineDriverTest::test_registerSwitcherData()
{
    try {
    DataRequirements req;
    req.addRequired("TestDataBlob");

    // Create the data client.
    ConfigNode config;
    DataSpec spec;
    spec.addStreamData("TestDataBlob");
    TestDataClient client(config, spec);
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
    catch(const QString& e ) {
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
    req.addRequired("TestDataBlob");
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
        req.addRequired("FloatData");
        TestPipeline *pipeline = new TestPipeline(req, num);
        _pipelineDriver->registerPipeline(pipeline);
        CPPUNIT_ASSERT_EQUAL(0, pipeline->count());

        // Create the data client.
        ConfigNode config;
        DataSpec spec;
        spec.addStreamData("FloatData");
        TestDataClient client(config, spec);
        _pipelineDriver->_dataClient = &client;

        // Start the pipeline driver.
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
        CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
    }
    catch (const QString& e) {
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
        pipelineReq.addRequired("FloatData");
        TestPipeline *pipeline = new TestPipeline(pipelineReq, num);
        _pipelineDriver->registerPipeline(pipeline);
        CPPUNIT_ASSERT_EQUAL(0, pipeline->count());

        // Create the data client.
        ConfigNode config;
        DataSpec clientTypes;
        clientTypes.addStreamData("OtherStreamData");
        TestDataClient client(config, clientTypes);
        _pipelineDriver->_dataClient = &client;

        // Start the pipeline driver.
        CPPUNIT_ASSERT_THROW(_pipelineDriver->start(), QString);
        CPPUNIT_ASSERT_EQUAL(0, pipeline->count());
        CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
    }
    catch (const QString& e) {
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
        pipelineReq1.addRequired(type1);
        pipelineReq2.addRequired(type2);
        TestPipeline *pipeline1 = new TestPipeline(pipelineReq1, num);
        TestPipeline *pipeline2 = new TestPipeline(pipelineReq2, num);
        _pipelineDriver->registerPipeline(pipeline1);
        _pipelineDriver->registerPipeline(pipeline2);
        CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());
        CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());

        // Create the data client.
        ConfigNode config;
        DataSpec clientTypes;
        clientTypes.addStreamData(type1); // Add both types of required data.
        clientTypes.addStreamData(type2); // Add both types of required data.
        TestDataClient client(config, clientTypes);
        _pipelineDriver->_dataClient = &client;

        // Start the pipeline driver.
        _pipelineDriver->start();
        CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
        CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
        CPPUNIT_ASSERT_EQUAL(num, pipeline2->count());
        CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());
    }
    catch (const QString& e) {
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
        pipelineReq1.addRequired(type1);
        pipelineReq2.addRequired(type2);
        TestPipeline *pipeline1 = new TestPipeline(pipelineReq1, num);
        TestPipeline *pipeline2 = new TestPipeline(pipelineReq2, num);
        _pipelineDriver->registerPipeline(pipeline1);
        _pipelineDriver->registerPipeline(pipeline2);
        CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());
        CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());

        // Create the data client.
        ConfigNode config;
        DataSpec clientTypes;
        clientTypes.addStreamData(type1); // Add one type of required data.
        TestDataClient client(config, clientTypes);
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
    catch (const QString& e) {
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
            pipelineReq.addRequired(type1);
            TestPipeline *pipeline1 = new TestPipeline(pipelineReq, num);
            pipeline1->setHistory(type1, history);
            _pipelineDriver->registerPipeline(pipeline1);
            CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());

            // Create the data client.
            ConfigNode config;
            DataSpec clientTypes;
            clientTypes.addStreamData(type1); // Add one type of required data.
            TestDataClient client(config, clientTypes);
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

void PipelineDriverTest::_setTestClient() {
    if ( ! _client  ) {
        ConfigNode config;
        DataSpec spec;
        spec.addStreamData("TestDataBlob");
        _client = new TestDataClient(config, spec);
    }
    _pipelineDriver->setDataClient( _client );
}

} // namespace pelican
