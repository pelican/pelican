#include "TestPipelineCalibrateImageTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineCalibrateImage.h"
#include <QCoreApplication>

#include "utility/pelicanTimer.h"
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
    TIMER_START
    int argc = 2;
    char *argv[] = {
            (char*)"pelican",
            (char*)"--config=data/TestPipelineCalibrateImage.xml"
    };
    QCoreApplication app(argc, argv);

    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineCalibrateImage);
    pApp.setDataClient("FileDataClient");
    pApp.start();
    TIMER_STOP("TestPipelineCalibrateImageTest");
}

} // namespace pelican
