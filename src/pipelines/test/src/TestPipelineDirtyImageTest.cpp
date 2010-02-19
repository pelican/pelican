#include "TestPipelineDirtyImageTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/TestPipelineDirtyImage.h"
#include <QCoreApplication>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TestPipelineDirtyImageTest );
/**
 *@details TestPipelineDirtyImageTest
 */
TestPipelineDirtyImageTest::TestPipelineDirtyImageTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
TestPipelineDirtyImageTest::~TestPipelineDirtyImageTest()
{
}

void TestPipelineDirtyImageTest::setUp()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

void TestPipelineDirtyImageTest::tearDown()
{
    delete _app;
}

void TestPipelineDirtyImageTest::test_method()
{
    int argc = 2;
    char *argv[] = {
            (char*)"pelican",
            (char*)"--config=data/TestPipelineDirtyImage.xml"
    };
    QCoreApplication app(argc, argv);

    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new TestPipelineDirtyImage);
    pApp.setDataClient("FileDataClient");
    pApp.start();
}

} // namespace pelican
