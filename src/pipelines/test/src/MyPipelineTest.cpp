#include "MyPipelineTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "pipelines/MyPipeline.h"
#include <QCoreApplication>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( MyPipelineTest );
// class MyPipelineTest
MyPipelineTest::MyPipelineTest()
    : CppUnit::TestFixture()
{
}

MyPipelineTest::~MyPipelineTest()
{
}

void MyPipelineTest::setUp()
{
}

void MyPipelineTest::tearDown()
{
}

void MyPipelineTest::test_method()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    QCoreApplication app(argc, argv);

    PipelineApplication pApp(argc, argv);
    pApp.registerPipeline(new MyPipeline);
    pApp.setDataClient("FileDataClient");
    pApp.start();


}

} // namespace pelican
