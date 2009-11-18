#include "PipelineDriverTest.h"
#include "PipelineDriver.h"
#include "TestPipeline.h"
#include "DataRequirements.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PipelineDriverTest );
// class PipelineDriverTest 
PipelineDriverTest::PipelineDriverTest()
    : CppUnit::TestFixture()
{
}

PipelineDriverTest::~PipelineDriverTest()
{
}

void PipelineDriverTest::setUp()
{
    pipelineDriver = new PipelineDriver();
}

void PipelineDriverTest::tearDown()
{
    delete pipelineDriver;
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
}

void PipelineDriverTest::test_emptyPipeline()
{
    // Use Case:
    // Attempt to run a pipeline which has not been set up with registerPipeline.
    // Expected to throw an exception with a message.
    CPPUNIT_ASSERT_THROW(pipelineDriver->start(), QString);
}

void PipelineDriverTest::test_singlePipeline()
{
    // Use Case:
    // Attempt to run a single registered pipeline.
    // Expected to work with no exceptions.
    DataRequirements req;
    req.setStreamData("wibble");
    TestPipeline *pipeline = new TestPipeline(req);
    pipeline->setDriver(pipelineDriver);
    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->registerPipeline(pipeline));
    int num = 10;
    pipeline->setIterations(num);
    CPPUNIT_ASSERT_EQUAL(0, pipeline->count());
    CPPUNIT_ASSERT_NO_THROW(pipelineDriver->start());
    CPPUNIT_ASSERT_EQUAL(num, pipeline->count());
}

void PipelineDriverTest::test_multiPipeline()
{
}

} // namespace pelican
