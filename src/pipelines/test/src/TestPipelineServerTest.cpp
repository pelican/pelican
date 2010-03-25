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
#include "utility/pelicanTimer.h"
#include "utility/TestConfig.h"
#include <QCoreApplication>

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

void TestPipelineServerTest::test_method()
{
    try {
    _createConfig();
    TestConfig config("TestPipelineServer.xml", "pipelines");
    int argc = 2;
    char** argv = config.argv("TestPipelineServer.xml", "pipelines");

    QCoreApplication app(argc, argv);

    // Set up the server.
    PelicanServer server;
    DataManager dataManager;
    TestChunker chunker(&dataManager, "VisibilityData", false, 10);
    StreamDataBuffer* dataBuffer = new StreamDataBuffer("VisibilityData");

    dataManager.setStreamDataBuffer("VisibilityData", dataBuffer);
//    WritableData writableData = dataBuffer->getWritable(10);
//    std::cout << "returned writable data" << std::endl;
    server.addStreamChunker(&chunker, "127.0.0.1", 2001);

    // Add the protocol.
    AbstractProtocol* protocol = new TestProtocol("VisibilityData", ServerRequest::StreamData);
    server.addProtocol(protocol, 2000);

    // Start the server.
    server.start();
    while (!server.isReady()) {}
    std::cout << "Server ready." << std::endl;
//    writableData.data();

    // Start the pipeline binary.
    PipelineBinaryEmulator pipelineBinary(static_cast<Config*>(&config));

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

PipelineBinaryEmulator::PipelineBinaryEmulator(Config* config) : QThread()
{
    _config = config;
    start();
}

void PipelineBinaryEmulator::run()
{
    std::cout << "Starting binary thread" << std::endl;
    // Set up the client.
    QList<DataRequirements> reqList;
    DataRequirements req;
    req.addStreamData("VisibilityData");
    reqList.append(req);
    Config::TreeAddress_t base;
    base.append(Config::NodeId_t("clients", ""));
    AdapterFactory adapterFactory(_config);
    DataClientFactory clientFactory(_config, base, &adapterFactory);
    AbstractDataClient* dataClient = clientFactory.create("PelicanServerClient", reqList);
    VisibilityData* visData = new VisibilityData;
    QHash<QString, DataBlob*> hash, validHash;
    hash.insert("VisibilityData", visData);
    while (validHash.isEmpty()) {
        std::cout << "Client getting data" << std::endl;
        validHash = dataClient->getData(hash);
        std::cout << "Client get data done." << std::endl;
        msleep(1000);
    }
    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FINISHED" << std::endl;

    // Clean up.
    delete visData;

    // Set up the pipeline.
//    PipelineApplication pApp(argc, argv);
//    pApp.registerPipeline(new TestPipelineServer);
//    pApp.setDataClient("FileDataClient");
//    pApp.start();
}

} // namespace pelican
