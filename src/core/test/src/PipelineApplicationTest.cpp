#include "PipelineApplicationTest.h"
#include "PipelineApplication.h"
#include <QCoreApplication>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PipelineApplicationTest );
// class PipelineApplicationTest 
PipelineApplicationTest::PipelineApplicationTest()
    : CppUnit::TestFixture()
{
}

PipelineApplicationTest::~PipelineApplicationTest()
{
}

void PipelineApplicationTest::setUp()
{
}

void PipelineApplicationTest::tearDown()
{
}

void PipelineApplicationTest::test_noQApplication()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican", (char*)"test"};
    PipelineApplication* pApp = 0;

    CPPUNIT_ASSERT_THROW( new PipelineApplication(argc, argv), QString );

    QCoreApplication app( argc, argv  );
    CPPUNIT_ASSERT_NO_THROW( pApp = new PipelineApplication(argc, argv) );
    delete pApp;
}

} // namespace pelican
