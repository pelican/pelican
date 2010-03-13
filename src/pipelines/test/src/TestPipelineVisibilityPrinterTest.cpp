#include "TestPipelineVisibilityPrinterTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineVisibilityPrinter.h"
#include "utility/pelicanTimer.h"
#include "utility/TestConfig.h"
#include <QCoreApplication>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TestPipelineVisibilityPrinterTest );
// class TestPipelineVisibilityPrinterTest
TestPipelineVisibilityPrinterTest::TestPipelineVisibilityPrinterTest()
    : CppUnit::TestFixture()
{
}

TestPipelineVisibilityPrinterTest::~TestPipelineVisibilityPrinterTest()
{
}

void TestPipelineVisibilityPrinterTest::setUp()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

void TestPipelineVisibilityPrinterTest::tearDown()
{
    delete _app;
}

void TestPipelineVisibilityPrinterTest::test_method()
{
    TestConfig config;
    int argc = 2;
    char** argv = config.argv("TestPipelineVisibilityPrinter.xml", "pipelines");

    QCoreApplication app(argc, argv);
    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineVisibilityPrinter);
    pApp.setDataClient("FileDataClient");
    pApp.start();
}

} // namespace pelican
