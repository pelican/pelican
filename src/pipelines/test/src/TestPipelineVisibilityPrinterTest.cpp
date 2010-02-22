#include "TestPipelineVisibilityPrinterTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineVisibilityPrinter.h"
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
    int argc = 2;
    char *argv[] = {(char*)"pelican", (char*)"--config=data/TestPipelineVisibilityPrinter.xml"};
    QCoreApplication app(argc, argv);

    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineVisibilityPrinter);
    pApp.setDataClient("FileDataClient");
    pApp.start();
}

} // namespace pelican
