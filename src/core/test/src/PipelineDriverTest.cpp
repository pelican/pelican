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
// class PipelineDriverTest 
PipelineDriverTest::PipelineDriverTest()
    : CppUnit::TestFixture()
{
    pipelineDriver = NULL;
}

PipelineDriverTest::~PipelineDriverTest()
{
}

void PipelineDriverTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    coreApp = new QCoreApplication(argc, argv);
    _dataBlobFactory = new DataBlobFactory;
    _moduleFactory = new ModuleFactory(0);
    Config::TreeAddress_t clientsNode;
    _clientFactory = new DataClientFactory(0,clientsNode, 0);
    pipelineDriver = new PipelineDriver(_dataBlobFactory, _moduleFactory, _clientFactory);
}

void PipelineDriverTest::tearDown()
{
    delete _dataBlobFactory;
    delete _moduleFactory;
    delete _clientFactory;
    delete pipelineDriver;
    delete coreApp;
}

void PipelineDriverTest::test_registerPipeline()
{
    // Use Case:
    // Pipeline requiring no data.
    // Expected to throw an exception and delete the passed object.
    CPPUNIT_ASSERT_THROW(pipelineDriver->registerPipeline(new TestPipeline), QString);

    // Use Case:
    // Pipeline requiring some data.
    // Expected to not throw any exceptions.
    DataRequirements req;
    req.setStreamData("wibble");
    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->registerPipeline(new TestPipeline(req)));

    // Use Case:
    // Try to register two pipelines requiring the same stream data.
    // Expect an exception to be thrown.
    DataRequirements req1;
    req1.setStreamData("wibble");
    CPPUNIT_ASSERT_THROW(pipelineDriver->registerPipeline(new TestPipeline(req1)), QString);
}

void PipelineDriverTest::test_emptyPipeline()
{
    return;
    // Use Case:
    // Attempt to run a pipeline which has not been set up with registerPipeline.
    // Expected to throw an exception with a message.
    CPPUNIT_ASSERT_THROW(pipelineDriver->start(), QString);
}

void PipelineDriverTest::test_singlePipelineInvalidData()
{
    return;
    // Use Case:
    // Attempt to run a pipeline which has been set up
    // but the data returned by getData() does not match any of the pipelines.
    // Expected to throw an exception with a message.
    DataRequirements req;
    req.setStreamData("wibble");
    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->registerPipeline(new TestPipeline(req)));
    CPPUNIT_ASSERT_THROW(pipelineDriver->start(), QString);
}

void PipelineDriverTest::test_singlePipeline()
{
    // Use Case:
    // Attempt to run a single registered pipeline.
    // Expect run method to be called with appropriate data on the test pipeline (repeatedly).

    QString wibble("wibble");
//    TestDataClient client;
//    QSet<QString> set;
//    set.insert(wibble);
//    client.setSubset(set);
    pipelineDriver->setDataClient("Test");

    DataRequirements req;
    req.setStreamData(wibble);
    TestPipeline *pipeline = new TestPipeline(req);
    pipeline->setPipelineDriver(pipelineDriver);
    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->registerPipeline(pipeline));
    int num = 10;
    pipeline->setIterations(num);
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());
    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
    CPPUNIT_ASSERT_EQUAL(pipeline->count(), pipeline->matchedCounter());
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
//    pipelineDriver->setDataClient(&client);
//
//    DataRequirements req1;
//    DataRequirements req2;
//    req1.setStreamData(reqData1);
//    req2.setStreamData(reqData2);
//    TestPipeline *pipeline1 = new TestPipeline(req1);
//    TestPipeline *pipeline2 = new TestPipeline(req2);
//    pipeline1->setPipelineDriver(pipelineDriver);
//    pipeline2->setPipelineDriver(pipelineDriver);
//    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->registerPipeline(pipeline1));
//    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->registerPipeline(pipeline2));
//    int num = 10;
//    pipeline1->setIterations(num);
//    pipeline2->setIterations(num);
//    CPPUNIT_ASSERT_EQUAL(0, pipeline1->count());
//    CPPUNIT_ASSERT_EQUAL(0, pipeline2->count());
//
//    // Use Case:
//    // Data returned matches both pipelines.
//    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->start());
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
//    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->start());
//    CPPUNIT_ASSERT_EQUAL(num, pipeline1->count());
//    CPPUNIT_ASSERT_EQUAL(num, pipeline2->count());
//    CPPUNIT_ASSERT_EQUAL(pipeline1->count(), pipeline1->matchedCounter());
//    CPPUNIT_ASSERT_EQUAL(pipeline2->count(), pipeline2->matchedCounter());

}

} // namespace pelican
