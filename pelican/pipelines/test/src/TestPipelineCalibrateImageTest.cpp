#include "pelican/pipelines/TestPipelineCalibrateImage.h"
#include "pelican/pipelines/test/TestPipelineCalibrateImageTest.h"
#include "pelican/core/PipelineApplication.h"
#include "pelican/core/AbstractPipeline.h"
#include "pelican/utility/pelicanTimer.h"
#include "pelican/utility/TestConfig.h"
#include <QCoreApplication>

#include "pelican/utility/memCheck.h"

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
//    int ac = 0;
//    _app = new QCoreApplication(ac, NULL);
}

void TestPipelineCalibrateImageTest::tearDown()
{
//    delete _app;
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
