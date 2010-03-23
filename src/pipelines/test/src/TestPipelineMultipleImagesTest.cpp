#include "pipelines/test/TestPipelineMultipleImagesTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineMultipleImages.h"
#include <QCoreApplication>
#include "utility/pelicanTimer.h"
#include "utility/TestConfig.h"

#include "utility/memCheck.h"

namespace pelican {


CPPUNIT_TEST_SUITE_REGISTRATION( TestPipelineMultipleImagesTest );


/**
 *@details TestPipelineMultipleImages
 */
TestPipelineMultipleImagesTest::TestPipelineMultipleImagesTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
TestPipelineMultipleImagesTest::~TestPipelineMultipleImagesTest()
{
}


void TestPipelineMultipleImagesTest::setUp()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}


void TestPipelineMultipleImagesTest::tearDown()
{
    delete _app;
}


void TestPipelineMultipleImagesTest::test_method()
{
    TestConfig config;
    int argc = 2;
    char** argv = config.argv("TestPipelineMultipleImages.xml", "pipelines");

    TIMER_START
    QCoreApplication app(argc, argv);
    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineMultipleImages);
    pApp.setDataClient("FileDataClient");
    pApp.start();
    TIMER_STOP("TestPipelineMultipleImages");
}

} // namespace pelican
