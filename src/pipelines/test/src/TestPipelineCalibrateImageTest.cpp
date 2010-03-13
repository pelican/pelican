#include "TestPipelineCalibrateImageTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineCalibrateImage.h"
#include "utility/pelicanTimer.h"
#include "utility/TestConfig.h"
#include <QCoreApplication>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TestPipelineCalibrateImageTest );
/**
 *@details TestPipelineDirtyImageTest
 */
TestPipelineCalibrateImageTest::TestPipelineCalibrateImageTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
TestPipelineCalibrateImageTest::~TestPipelineCalibrateImageTest()
{
}

void TestPipelineCalibrateImageTest::setUp()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

void TestPipelineCalibrateImageTest::tearDown()
{
    delete _app;
}

void TestPipelineCalibrateImageTest::test_method()
{

    TestConfig config;
    int argc = 2;
    char** argv = config.argv("TestPipelineCalibrateImage.xml", "pipelines");

    TIMER_START
    QCoreApplication app(argc, argv);
    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineCalibrateImage);
    pApp.setDataClient("FileDataClient");
    pApp.start();
    TIMER_STOP("TestPipelineCalibrateImageTest");
}

} // namespace pelican
