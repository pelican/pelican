#include "DirectStreamDataClientTest.h"
#include "pelican/adapters/AbstractAdapter.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/core/DirectStreamDataClient.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/Factory.h"

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
    QString pipelineXml = ""
            "<chunkers>"
            "    <TestUdpChunker name=\"a\">"
            "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
            "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
            "    </TestUdpChunker>"
            "</chunkers>"
            "<clients>"
            "    <DirectStreamDataClient>"
            "        <data type=\"VisibilityData\" adapter=\"AdapterLofarStationVisibilities\"/>"
            // FIXME How do we specify chunkers for a non-generic data client?
            "        <chunker type=\"TestUdpChunker\" name=\"a\" data=\"VisibilityData\"/>"
            "        <chunker type=\"TestUdpChunker\" name=\"b\" data=\"VisibilityData\"/>"
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
}

void DirectStreamDataClientTest::test_method()
{
    // Create the adapter factory.
    Factory<AbstractAdapter> adapterFactory(_config, "pipeline", "adapters");

    // Create the data client factory.
    DataClientFactory clientFactory(_config, "pipeline", "clients", &adapterFactory);

    // Create a list of data requirements.
    DataRequirements req;
    QList<DataRequirements> requirements;
    req.addStreamData("VisibilityData");
    requirements.append(req);

    // Create the client.
    DirectStreamDataClient* client = static_cast<DirectStreamDataClient*>(
            clientFactory.create("DirectStreamDataClient", requirements));
//    client->setChunker("VisibilityData", "TestUdpChunker", "a");

    std::cout << "Testing Direct Stream Data Client" << std::endl;
}

} // namespace pelican
