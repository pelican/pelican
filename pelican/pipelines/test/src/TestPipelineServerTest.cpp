#include "pelican/pipelines/test/TestPipelineServerTest.h"
#include "pelican/core/PipelineApplication.h"
#include "pelican/pipelines/TestPipelineServer.h"
#include "pelican/server/PelicanServer.h"
#include "pelican/server/test/TestChunker.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/utility/pelicanTimer.h"
#include "pelican/utility/TestConfig.h"
#include "pelican/testutils/EmulatorDriver.h"
#include "pelican/testutils/RealUdpEmulator.h"
#include "pelican/server/test/TestUdpChunker.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include "pelican/utility/Config.h"

#include "pelican/utility/memCheck.h"

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
    // Set up the emulator configuration.
    _emulatorConfig1 = new ConfigNode;
    _emulatorConfig2 = new ConfigNode;
    _emulatorConfig1->setFromString(""
            "<RealUdpEmulator>"
            "    <connection host=\"127.0.0.1\" port=\"2002\"/>"
            "    <packet size=\"512\" interval=\"1000\" initialValue=\"0.1\"/>"
            "</RealUdpEmulator>"
            );
    _emulatorConfig2->setFromString(""
            "<RealUdpEmulator>"
            "    <connection host=\"127.0.0.1\" port=\"2003\"/>"
            "    <packet size=\"512\" interval=\"1000\" initialValue=\"0.2\"/>"
            "</RealUdpEmulator>"
            );
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
        PelicanServer server(&config);
        server.addStreamChunker("TestChunker");

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);

        // Start the server.
        server.start();
        while (!server.isReady()) {}

        // Start the pipeline binary.
        PipelineBinaryEmulator pipelineBinary(&config);

        // Return after 12 seconds.
        QTimer::singleShot(30000, &app, SLOT(quit()));
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
        PelicanServer server(&config);
        server.addStreamChunker("TestUdpChunker", "1");

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);
        
        // Start the server.
        server.start();
        while (!server.isReady()) {}
        
        // Set up the telescope emulator (turn on the telescope)
        EmulatorDriver emulator(new RealUdpEmulator(*_emulatorConfig1));
        
        // Start the pipeline binary.
        PipelineBinaryEmulator pipelineBinary(&config);
        
        // Return after 12 seconds.
        QTimer::singleShot(30000, &app, SLOT(quit()));
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
    // Set up the telescope emulators (turn on the telescope)
    EmulatorDriver emulator1(new RealUdpEmulator(*_emulatorConfig1));
    EmulatorDriver emulator2(new RealUdpEmulator(*_emulatorConfig2));

    for (int i = 0; i < 10; i++) {
    try {
        _createConfig();
        TestConfig config("TestPipelineServer.xml", "pipelines");
        int argc = 2;
        char** argv = config.argv("TestPipelineServer.xml", "pipelines");

        QCoreApplication app(argc, argv);

        // Set up the server.
        PelicanServer server(&config);
        server.addStreamChunker("TestUdpChunker", "1");
        server.addStreamChunker("TestUdpChunker", "2");

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);

        // Start the server.
        server.start();
        while (!server.isReady()) {}

        // Start the pipeline binary.
        PipelineBinaryEmulator pipelineBinary(&config);

        // Return after 12 seconds.
        QTimer::singleShot(30000, &app, SLOT(quit()));
        app.exec();
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
    }
}


void TestPipelineServerTest::_createConfig()
{
    TestConfig config;

    QString pipelineXml =
    "<clients>"
    "   <PelicanServerClient>"
    "       <server host=\"127.0.0.1\" port=\"2000\"/>"
    "   <data type=\"VisibilityData\" adapter=\"AdapterLofarStationVisibilities\"/>"
    "   </PelicanServerClient>"
    "</clients>"
    ""
    "<adapters>"
    "   <AdapterLofarStationVisibilities>"
    "       <antennas number=\"2\"/>"
    "       <channels start=\"0\" end=\"1\"/>"
    "       <polarisation value=\"both\"/>"
    "       <dataBytes number=\"8\"/>"
    "   </AdapterLofarStationVisibilities>"
    "</adapters>";

    QString serverXml =
    "<buffers>"
    "   <VisibilityData>"
    "       <buffer maxSize=\"10000\" maxChunkSize=\"10000\"/>"
    "   </VisibilityData>"
    "</buffers>"
    ""
    "<chunkers>"
    "   <TestChunker>"
    "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
    "   </TestChunker>"
    "   <TestUdpChunker name=\"1\">"
    "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
    "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
    "   </TestUdpChunker>"
    "   <TestUdpChunker name=\"2\">"
    "       <connection host=\"127.0.0.1\" port=\"2003\"/>"
    "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
    "   </TestUdpChunker>"
    "</chunkers>";

    config.setFromString(pipelineXml, serverXml);
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
