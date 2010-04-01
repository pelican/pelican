#include "pipelines/test/TestPipelineServerTest.h"
#include "core/PipelineApplication.h"
#include "pipelines/TestPipelineServer.h"
#include "server/PelicanServer.h"
#include "server/test/TestChunker.h"
#include "comms/PelicanProtocol.h"
#include "utility/pelicanTimer.h"
#include "utility/TestConfig.h"
#include "server/test/TelescopeEmulator.h"
#include "server/test/TestUdpChunker.h"
#include <QCoreApplication>
#include <QTimer>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( TestPipelineServerTest );

/**
 * @details
 * TestPipelineServerTest constructor.
 */
TestPipelineServerTest::TestPipelineServerTest()
: CppUnit::TestFixture()
{
}

/**
 * @details
 * Destroys the TestPipelineServerTest.
 */
TestPipelineServerTest::~TestPipelineServerTest()
{
}

/**
 * @details
 * Set-up routine run before each test.
 */
void TestPipelineServerTest::setUp()
{
}

/**
 * @details
 * Clean-up routine run after each test.
 */
void TestPipelineServerTest::tearDown()
{
}

/**
 * @details
 * Creates a Pelican server with a TestChunker, and a test pipeline.
 * Expect the test pipeline to be called with data from the chunker.
 */
void TestPipelineServerTest::test_testChunker()
{
    try {
        _createConfig();
        TestConfig config("TestPipelineServer.xml", "pipelines");
        int argc = 2;
        char** argv = config.argv("TestPipelineServer.xml", "pipelines");

        QCoreApplication app(argc, argv);

        // Set up the server.
        PelicanServer server;
        TestChunker* chunker = new TestChunker("VisibilityData", false, 512);
        server.addStreamChunker(chunker);

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);

        // Start the server.
        server.start();
        while (!server.isReady()) {}

        // Start the pipeline binary.
        PipelineBinaryEmulator pipelineBinary(&config);

        // Return after 12 seconds.
        QTimer::singleShot(12000, &app, SLOT(quit()));
        app.exec();
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

/**
 * @details
 * Creates a Pelican server with a TestUdpChunker, a telescope emulator,
 * and a test pipeline.
 * Expect the test pipeline to be called with data from the telescope emulator.
 */
void TestPipelineServerTest::test_testUdpChunker()
{
    try {
        _createConfig();
        TestConfig config("TestPipelineServer.xml", "pipelines");
        int argc = 2;
        char** argv = config.argv("TestPipelineServer.xml", "pipelines");

        QCoreApplication app(argc, argv);

        // Set up the server.
        qint16 telescopePort = 2002;
        PelicanServer server;
        TestUdpChunker* chunker = new TestUdpChunker("VisibilityData",
                            "127.0.0.1", telescopePort, 512);
        server.addStreamChunker(chunker);

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);
        
        // Start the server.
        server.start();
        while (!server.isReady()) {}
        
        // Set up the telescope emulator (turn on the telescope)
        TelescopeEmulator telescope(telescopePort, 0.5);
        
        // Start the pipeline binary.
        PipelineBinaryEmulator pipelineBinary(&config);
        
        // Return after 12 seconds.
        QTimer::singleShot(12000, &app, SLOT(quit()));
        app.exec();
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

/**
 * @details
 * Creates a Pelican server with two TestUdpChunkers, two telescope emulators,
 * and a test pipeline.
 * Expect the test pipeline to be called with data from the telescope emulator.
 */
void TestPipelineServerTest::test_testTwoUdpChunkers()
{
    try {
        _createConfig();
        TestConfig config("TestPipelineServer.xml", "pipelines");
        int argc = 2;
        char** argv = config.argv("TestPipelineServer.xml", "pipelines");

        QCoreApplication app(argc, argv);

        // Set up the telescope emulators (turn on the telescope)
        qint16 telescopePort1 = 2002, telescopePort2 = 2003;
        TelescopeEmulator telescope1(telescopePort1, 0.1);
        TelescopeEmulator telescope2(telescopePort2, 0.2);

        // Set up the server.
        PelicanServer server;
        TestUdpChunker* chunker1 = new TestUdpChunker("VisibilityData",
                            "127.0.0.1", telescopePort1, 512);
        TestUdpChunker* chunker2 = new TestUdpChunker("VisibilityData",
                            "127.0.0.1", telescopePort2, 512);
        server.addStreamChunker(chunker1);
        server.addStreamChunker(chunker2);

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);

        // Start the server.
        server.start();
        while (!server.isReady()) {}

        // Start the pipeline binary.
        PipelineBinaryEmulator pipelineBinary(&config);

        // Return after 12 seconds.
        QTimer::singleShot(12000, &app, SLOT(quit()));
        app.exec();
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

void TestPipelineServerTest::_createConfig()
{
    TestConfig config;

    QString xml =
    "<clients>"
    "   <PelicanServerClient>"
    "       <server host=\"127.0.0.1\" port=\"2000\"/>"
    "       <data type=\"VisibilityData\" adapter=\"AdapterLofarStationVisibilities\"/>"
    "   </PelicanServerClient>"
    "</clients>"
    "<adapters>"
    "   <AdapterLofarStationVisibilities>"
    "       <antennas number=\"2\"/>"
    "       <channels start=\"0\" end=\"1\"/>"
    "       <polarisation value=\"both\"/>"
    "       <dataBytes number=\"8\"/>"
    "   </AdapterLofarStationVisibilities>"
    "</adapters>";

    config.setFromString(xml);
    config.saveTestConfig("TestPipelineServer.xml", "pipelines");
}


void PipelineBinaryEmulator::run()
{
    // Set up the pipeline.
    int argc = 2;
    PipelineApplication pApp(argc, _config->argv("TestPipelineServer.xml", "pipelines"));
    pApp.registerPipeline(new TestPipelineServer);
    pApp.setDataClient("PelicanServerClient");
    pApp.start();
}

} // namespace pelican
