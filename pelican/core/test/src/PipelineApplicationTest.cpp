#include "PipelineApplicationTest.h"
#include "pelican/core/PipelineApplication.h"

#include <QtCore/QCoreApplication>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PipelineApplicationTest );

/**
 * @details
 * PipelineApplicationTest constructor.
 */
PipelineApplicationTest::PipelineApplicationTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * PipelineApplicationTest destructor.
 */
PipelineApplicationTest::~PipelineApplicationTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void PipelineApplicationTest::setUp()
{
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void PipelineApplicationTest::tearDown()
{
}

/**
 * @details
 * Try to create a PipelineApplication before a QCoreApplication.
 * Expect an exception.
 */
void PipelineApplicationTest::test_create_beforeQCoreApplication()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    CPPUNIT_ASSERT_THROW(PipelineApplication(argc, argv), QString);
}

/**
 * @details
 * Try to create a PipelineApplication after a QCoreApplication.
 * Expect no exceptions.
 */
void PipelineApplicationTest::test_create_afterQCoreApplication()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    QCoreApplication app(argc, argv);
    CPPUNIT_ASSERT_NO_THROW(PipelineApplication(argc, argv));
}

/**
 * @details
 * Try to create a PipelineApplication with a configuration file name.
 * Expect no exceptions.
 */
void PipelineApplicationTest::test_commandLine_config()
{
    int argc = 2;
    char *argv[] = {(char*)"pelican", (char*)"--config=settings.xml"};

    QCoreApplication app(argc, argv);
    CPPUNIT_ASSERT_NO_THROW(PipelineApplication(argc, argv));
}

/**
 * @details
 * Try to create a PipelineApplication with --help on the command line.
 * Expect no exceptions.
 */
void PipelineApplicationTest::test_commandLine_help()
{
    int argc = 2;
    char *argv[] = {(char*)"pelican", (char*)"--help"};

    QCoreApplication app(argc, argv);
    CPPUNIT_ASSERT_NO_THROW(PipelineApplication(argc, argv));
}

/**
 * @details
 * Try to create a PipelineApplication and register an empty pipeline.
 * Expect an exception.
 */
void PipelineApplicationTest::test_registerPipeline_EmptyPipeline()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    QCoreApplication app(argc, argv);
    PipelineApplication *pApp = NULL;
    CPPUNIT_ASSERT_NO_THROW(pApp = new PipelineApplication(argc, argv));

    // TODO register the empty pipeline.

    delete pApp;
}

/**
 * @details
 * Try to create a PipelineApplication, register a pipeline and start it.
 */
void PipelineApplicationTest::test_start()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};

    QCoreApplication app(argc, argv);
    PipelineApplication *pApp = NULL;
    CPPUNIT_ASSERT_NO_THROW(pApp = new PipelineApplication(argc, argv));

    // TODO register a pipeline and start it.

    delete pApp;
}

} // namespace pelican
