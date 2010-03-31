#include "pipelines/test/TestPipelineServerTest.h"
#include "core/PipelineApplication.h"
#include "core/AbstractPipeline.h"
#include "core/AbstractDataClient.h"
#include "core/DataClientFactory.h"
#include "data/VisibilityData.h"
#include "adapters/AdapterFactory.h"
#include "pipelines/TestPipelineServer.h"
#include "server/PelicanServer.h"
#include "server/DataManager.h"
#include "server/WritableData.h"
#include "server/test/TestChunker.h"
#include "server/test/PelicanTestClient.h"
#include "server/test/TestProtocol.h"
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
// class TestPipelineServerTest
TestPipelineServerTest::TestPipelineServerTest()
: CppUnit::TestFixture()
{
}

TestPipelineServerTest::~TestPipelineServerTest()
{
}

void TestPipelineServerTest::setUp()
{
}

void TestPipelineServerTest::tearDown()
{
}

void TestPipelineServerTest::test_testChunker()
{
    try {
        std::cout << "TestPipelineServerTest::test_testChunker()" << std::endl;
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


void TestPipelineServerTest::test_testUdpChunker()
{
    try {
        _createConfig();
        TestConfig config("TestPipelineServer.xml", "pipelines");
        int argc = 2;
        char** argv = config.argv("TestPipelineServer.xml", "pipelines");

        QCoreApplication app(argc, argv);

        // Set up the server.
        PelicanServer server;
        TestUdpChunker* chunker = new TestUdpChunker("VisibilityData",
                                                     "127.0.0.1", 2002, 512);
        server.addStreamChunker(chunker);

        // Add the protocol.
        AbstractProtocol* protocol = new PelicanProtocol;
        server.addProtocol(protocol, 2000);
        
        // Start the server.
        server.start();
        while (!server.isReady()) {}
        
        // Set up the telescope emulator (turn on the telescope)
        TelescopeEmulator telescope(2002, 0.5);
        
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


void TestPipelineServerTest::test_testTwoUdpChunkers()
{
    try {
        _createConfig();
        TestConfig config("TestPipelineServer.xml", "pipelines");
        int argc = 2;
        char** argv = config.argv("TestPipelineServer.xml", "pipelines");

        QCoreApplication app(argc, argv);

        // Set up the telescope emulators (turn on the telescope)
        qint16 telescopePort1 = 2002;
        qint16 telescopePort2 = 2003;
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


PipelineBinaryEmulator::PipelineBinaryEmulator(TestConfig* config) : QThread()
{
    _config = config;
    start();
}


void PipelineBinaryEmulator::run()
{
    try {
//        std::cout << "Starting binary thread" << std::endl;
//        // Set up the client.
//        QList<DataRequirements> reqList;
//        DataRequirements req;
//        req.addStreamData("VisibilityData");
//        reqList.append(req);
//        Config::TreeAddress_t base;
//        base.append(Config::NodeId_t("clients", ""));
//        AdapterFactory adapterFactory(_config);
//        DataClientFactory clientFactory(_config, base, &adapterFactory);
//        AbstractDataClient* dataClient = clientFactory.create("PelicanServerClient", reqList);
//        VisibilityData* visData = new VisibilityData;
//        QHash<QString, DataBlob*> hash, validHash;
//        hash.insert("VisibilityData", visData);
//        while (validHash.isEmpty()) {
//            std::cout << "Client getting data" << std::endl;
//            validHash = dataClient->getData(hash);
//            std::cout << "Client get data done." << std::endl;
//            //        msleep(500);
//        }
//        std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FINISHED" << std::endl;
//
//        complex_t* dat =  visData->ptr();
//        std::cout << "Number of entries: " << visData->nEntries();
//        for (unsigned i = 0; i  < visData->nEntries(); ++i) {
//            std::cout << dat[i] << std::endl;
//        }
//
//        // Clean up.
//        delete visData;

        // Set up the pipeline.
        int argc = 2;
        PipelineApplication pApp(argc, _config->argv("TestPipelineServer.xml", "pipelines"));
        pApp.registerPipeline(new TestPipelineServer);
        pApp.setIgnoreEmptyHash(true);
        pApp.setDataClient("PelicanServerClient");
        pApp.start();
    }
    catch (QString e) {
        std::cerr << "Unexpected exception: " + e.toStdString() << std::endl;
    }
}

} // namespace pelican
