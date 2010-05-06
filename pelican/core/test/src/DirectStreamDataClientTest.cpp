#include "DirectStreamDataClientTest.h"
#include "pelican/adapters/AbstractAdapter.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/core/DirectStreamDataClient.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/VisibilityData.h"
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
            "    <TestUdpChunker name=\"b\">"
            "       <connection host=\"127.0.0.1\" port=\"2003\"/>"
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

void DirectStreamDataClientTest::test_singleChunker()
{
    std::cout << "----------------------------------" << std::endl;
    try {
        // Start the telescope emulator on port 2002.
        TelescopeEmulator telescope1(2002, QHostAddress::LocalHost, 0.2);

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

        // Get the data.
        QHash<QString, DataBlob*> validData = client->getData(dataHash);

        // Check the content of the data blob.
        VisibilityData* visData = (VisibilityData*)validData.value("VisibilityData");
        _printVisibilities(visData);
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

/**
 * @details
 * Tests two chunkers, starting them multiple times.
 */
void DirectStreamDataClientTest::test_twoChunkersMultipleStarts()
{
    for (int i = 0; i < 1000; ++i) {
        std::cout << "---------------------------------- " << i << std::endl;
        try {
            // Start two telescope emulators.
            TelescopeEmulator telescope1(2002, QHostAddress::LocalHost, 0.2);
            TelescopeEmulator telescope2(2003, QHostAddress::LocalHost, 0.4);

            // Create the adapter factory.
            Factory<AbstractAdapter> adapterFactory(_config,
                    "pipeline", "adapters");

            // Create the data client factory.
            DataClientFactory clientFactory(_config, "pipeline",
                    "clients", &adapterFactory);

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
            client->addStreamChunker("TestUdpChunker", "b");
            client->start();

            // Set up the data hash.
            QHash<QString, DataBlob*> dataHash;
            dataHash.insert(dataType, blobFactory.create(dataType));

            for (int j = 0; j < 2; j++) {
                // Get the data.
                QHash<QString, DataBlob*> validData = client->getData(dataHash);

                // Check the content of the data blob.
                VisibilityData* visData = (VisibilityData*)validData.value("VisibilityData");
                _printVisibilities(visData);
            }
        }
        catch (QString e) {
            CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
        }
    }
}

/**
 * @details
 * Tests two chunkers, starting them only once.
 */
void DirectStreamDataClientTest::test_twoChunkersSingleStart()
{
    try {
        // Start two telescope emulators.
        TelescopeEmulator telescope1(2002, QHostAddress::LocalHost, 0.2);
        TelescopeEmulator telescope2(2003, QHostAddress::LocalHost, 0.4);

        // Create the adapter factory.
        Factory<AbstractAdapter> adapterFactory(_config,
                "pipeline", "adapters");

        // Create the data client factory.
        DataClientFactory clientFactory(_config, "pipeline",
                "clients", &adapterFactory);

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
        client->addStreamChunker("TestUdpChunker", "b");
        client->start();

        // Set up the data hash.
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(dataType, blobFactory.create(dataType));

        for (int i = 0; i < 2000; i++) {
            std::cout << "---------------------------------- " << i << std::endl;
            // Get the data.
            QHash<QString, DataBlob*> validData = client->getData(dataHash);

            // Check the content of the data blob.
            VisibilityData* visData = (VisibilityData*)validData.value("VisibilityData");
            _printVisibilities(visData);
        }
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

/**
 * @details
 * Prints the contents of a visibility data blob.
 *
 * @param visData
 */
void DirectStreamDataClientTest::_printVisibilities(VisibilityData* visData)
{
    CPPUNIT_ASSERT(visData); // Check that the blob exists.
    CPPUNIT_ASSERT(visData->nAntennas() != 0);

    unsigned nChan = visData->nChannels();
    unsigned nAnt = visData->nAntennas();
    unsigned nPol = visData->nPolarisations();
    for (unsigned p = 0; p < nPol; p++) {
        for (unsigned c = 0; c < nChan; c++) {
            complex_t* ptr = visData->ptr(c, p);
            for (unsigned j = 0; j < nAnt; j++) {
                for (unsigned i = 0; i < nAnt; i++) {
                    std::cout << "VisData: " << ptr[i + j * nAnt] << std::endl;
                }
            }
        }
    }
}

} // namespace pelican
