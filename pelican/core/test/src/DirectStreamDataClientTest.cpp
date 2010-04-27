#include "DirectStreamDataClientTest.h"
#include "pelican/adapters/AbstractAdapter.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/core/DirectStreamDataClient.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/server/test/TelescopeEmulator.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/Factory.h"

#include <QCoreApplication>
#include <QList>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DirectStreamDataClientTest );

/**
 * @details DirectStreamDataClientTest
 */
DirectStreamDataClientTest::DirectStreamDataClientTest()
    : CppUnit::TestFixture()
{
    _app = NULL;
}

/**
 * @details
 */
DirectStreamDataClientTest::~DirectStreamDataClientTest()
{
}

/**
 * @details
 * Set-up routine called before running each test.
 */
void DirectStreamDataClientTest::setUp()
{
    if (QCoreApplication::instance() == NULL) {
        int argc = 1;
        char *argv[] = {(char*)"pelican"};
        _app = new QCoreApplication(argc, argv);
    }

    QString pipelineXml = ""
            "<buffers>"
            "   <VisibilityData>"
            "       <buffer maxSize=\"2000\" maxChunkSize=\"2000\"/>"
            "   </VisibilityData>"
            "</buffers>"
            "<chunkers>"
            "    <TestUdpChunker name=\"a\">"
            "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
            "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
            "    </TestUdpChunker>"
            "</chunkers>"
            "<clients>"
            "    <DirectStreamDataClient>"
            "        <data type=\"VisibilityData\" adapter=\"AdapterLofarStationVisibilities\"/>"
            "    </DirectStreamDataClient>"
            "</clients>"
            "<adapters>"
            "   <AdapterLofarStationVisibilities>"
            "       <antennas number=\"2\"/>"
            "       <channels start=\"0\" end=\"1\"/>"
            "       <polarisation value=\"both\"/>"
            "       <dataBytes number=\"8\"/>"
            "   </AdapterLofarStationVisibilities>"
            "</adapters>";
    _config = new Config;
    _config->setFromString(pipelineXml);
}

void DirectStreamDataClientTest::tearDown()
{
    delete _config;
    delete _app;
}

void DirectStreamDataClientTest::test_method()
{
    try {
        // Create the adapter factory.
        Factory<AbstractAdapter> adapterFactory(_config, "pipeline", "adapters");

        // Create the data client factory.
        DataClientFactory clientFactory(_config, "pipeline", "clients", &adapterFactory);

        // Create the data blob factory.
        Factory<DataBlob> blobFactory;

        // Create a list of data requirements.
        QString dataType = "VisibilityData";
        DataRequirements req;
        QList<DataRequirements> requirements;
        req.addStreamData(dataType);
        requirements.append(req);

        // Create the client.
        DirectStreamDataClient* client = static_cast<DirectStreamDataClient*>(
                clientFactory.create("DirectStreamDataClient", requirements));
        client->addStreamChunker("TestUdpChunker", "a");
        client->start();

        // Set up the data hash.
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(dataType, blobFactory.create(dataType));
        QHash<QString, DataBlob*> validData = client->getData(dataHash);
        foreach (QString key, validData.keys()) {
            std::cout << "Returned valid data: " << key.toStdString() << std::endl;
        }
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
