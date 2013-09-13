/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/test/DirectStreamDataClientTest.h"

#include "core/AbstractAdapter.h"
#include "core/AbstractAdapterFactory.h"
#include "core/DataClientFactory.h"
#include "core/DirectStreamDataClient.h"
#include "data/DataBlob.h"
#include "data/ArrayData.h"
#include "core/AdapterRealData.h"
#include "server/test/TestUdpChunker.h"

#include "data/DataSpec.h"
#include "emulator/EmulatorDriver.h"
#include "emulator/test/RealUdpEmulator.h"
#include "utility/Config.h"
#include "utility/FactoryConfig.h"
#include "utility/FactoryGeneric.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <iostream>

using namespace std;

namespace pelican {

using test::RealUdpEmulator;

CPPUNIT_TEST_SUITE_REGISTRATION(DirectStreamDataClientTest);

/**
 * @details DirectStreamDataClientTest
 */
DirectStreamDataClientTest::DirectStreamDataClientTest()
: CppUnit::TestFixture(), _config(0), _emulatorConfig1(0),
  _emulatorConfig2(0)
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
            "<buffers>"
            "   <DoubleData>"
            "       <buffer maxSize=\"2000\"/>"
            "   </DoubleData>"
            "</buffers>"
            "<chunkers>"
            "    <TestUdpChunker name=\"a\">"
            "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
            "       <data type=\"DoubleData\" chunkSize=\"512\"/>"
            "    </TestUdpChunker>"
            "    <TestUdpChunker name=\"b\">"
            "       <connection host=\"127.0.0.1\" port=\"2003\"/>"
            "       <data type=\"DoubleData\" chunkSize=\"512\"/>"
            "    </TestUdpChunker>"
            "</chunkers>"
            "<clients>"
            "    <DirectStreamDataClient>"
            "        <data type=\"DoubleData\" adapter=\"AdapterRealData\"/>"
            "    </DirectStreamDataClient>"
            "</clients>"
            "<adapters>"
            "   <AdapterRealData>"
            "       <antennas number=\"2\"/>"
            "       <channels start=\"0\" end=\"1\"/>"
            "       <polarisation value=\"both\"/>"
            "       <dataBytes number=\"8\"/>"
            "   </AdapterRealData>"
            "</adapters>";
    _config = new Config;
    _config->setFromString(pipelineXml);

    // Set up the emulator configuration.
    _emulatorConfig1 = new ConfigNode;
    _emulatorConfig2 = new ConfigNode;
    _emulatorConfig1->setFromString(""
            "<RealUdpEmulator>"
            "    <connection host=\"127.0.0.1\" port=\"2002\"/>"
            "    <packet size=\"512\" interval=\"1000\" initialValue=\"0.1\" number=\"-1\"/>"
            "</RealUdpEmulator>"
    );
    _emulatorConfig2->setFromString(""
            "<RealUdpEmulator>"
            "    <connection host=\"127.0.0.1\" port=\"2003\"/>"
            "    <packet size=\"512\" interval=\"1000\" initialValue=\"0.2\" number=\"-1\"/>"
            "</RealUdpEmulator>"
    );
}

void DirectStreamDataClientTest::tearDown()
{
    delete _config;
    delete _emulatorConfig1;
    delete _emulatorConfig2;
}

void DirectStreamDataClientTest::test_singleChunker()
{
    try {
        // Start the telescope emulator on port 2002.
        EmulatorDriver emulatorDriver(new RealUdpEmulator(*_emulatorConfig1));

        // Create the adapter factory.
        AbstractAdapterFactory adapterFactory(_config, "pipeline", "adapters");

        // Create the data client factory.
        DataClientFactory clientFactory(_config, "pipeline", "clients", &adapterFactory);

        // Create the data blob factory.
        FactoryGeneric<DataBlob> blobFactory(true);

        // Create a list of data requirements.
        QString dataType = "DoubleData";
        DataSpec req;
        QList<DataSpec> requirements;
        req.addStreamData(dataType);
        requirements.append(req);

        // Create the client.
        DirectStreamDataClient* client = static_cast<DirectStreamDataClient*>(
                clientFactory.create("DirectStreamDataClient"));
        CPPUNIT_ASSERT(DataClientFactory::whatIs(client) == "DirectStreamDataClient");
        client->reset( requirements );
        client->addStreamChunker("TestUdpChunker", "a");

        // Set up the data hash.
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(dataType, blobFactory.create(dataType));

        // FIXME is next not called as the emulator is finished and therefore
        // no data is available when the socket in the chunker is opened...

        // Get the data.
        QHash<QString, DataBlob*> validData = client->getData(dataHash);

        // Check the content of the data blob.
        DoubleData* data = (DoubleData*)validData.value("DoubleData");

        // TODO check data
        CPPUNIT_ASSERT(data->size() != 0);

        // Stop the emulator.
        emulatorDriver.abort();
    }
    catch (const QString& e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }

}

/**
 * @details
 * Tests two chunkers, starting them only once and calling getData()
 * multiple times.
 */
void DirectStreamDataClientTest::test_twoChunkersSingleStart()
{
    try {
        // Start two telescope emulators.
        EmulatorDriver emulator1(new RealUdpEmulator(*_emulatorConfig1));
        EmulatorDriver emulator2(new RealUdpEmulator(*_emulatorConfig2));

        // Create factories
        AbstractAdapterFactory adapterFactory(_config, "pipeline", "adapters");
        DataClientFactory clientFactory(_config, "pipeline", "clients",
                &adapterFactory);
        FactoryGeneric<DataBlob> blobFactory(true);

        // Create a list of data requirements.
        QString dataType = "DoubleData";
        DataSpec req;
        QList<DataSpec> requirements;
        req.addStreamData(dataType);
        requirements.append(req);

        // Create the client.
        DirectStreamDataClient* client = static_cast<DirectStreamDataClient*>(
                clientFactory.create("DirectStreamDataClient" ));
        CPPUNIT_ASSERT(DataClientFactory::whatIs(client) == "DirectStreamDataClient");
        client->reset( requirements );
        client->addStreamChunker("TestUdpChunker", "a");
        client->addStreamChunker("TestUdpChunker", "b");

        // Set up the data hash.
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(dataType, blobFactory.create(dataType));

        int n = _emulatorConfig1->getOption("packet", "number").toInt();
        for (int i = 0; i < n; i++) {
            // Get the data.
            QHash<QString, DataBlob*> validData = client->getData(dataHash);

            // Check the content of the data blob.
            DoubleData* data = (DoubleData*)validData.value("DoubleData");

            // TODO check data
            CPPUNIT_ASSERT(data->size() != 0);
        }
        emulator1.abort();
        emulator2.abort();
    }
    catch (const QString& e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}


/**
 * @details
 * Tests two chunkers, starting them multiple times (test for random segfaults
 * and threading problems).
 *
 * XXX this test is broken!
 *
 *    - random hangs in the data receiver.
 *    - the same data is not always returned!
 *
 */
void DirectStreamDataClientTest::test_twoChunkersMultipleStarts()
{
    // XXX this test sometimes hangs -
    // This is the behaviour of the DataReceiver
    int niter = 1;
    int numChunksToCheck = 3;

    for (int iter = 0; iter < niter; ++iter) {
        std::cout << std::endl << std::string(80,'-') << std::endl;
        std::cout << __PRETTY_FUNCTION__ << " - START " << iter << std::endl;
        // Start two telescope emulators.
        EmulatorDriver emulator1(new RealUdpEmulator(*_emulatorConfig1));
        EmulatorDriver emulator2(new RealUdpEmulator(*_emulatorConfig2));

        // Create the adapter factory.
        AbstractAdapterFactory adapterFactory(_config,
                "pipeline", "adapters");

        // Create the data client factory.
        DataClientFactory clientFactory(_config, "pipeline",
                "clients", &adapterFactory);

        // Create the data blob factory.
        FactoryGeneric<DataBlob> blobFactory(true);

        // Create a list of data requirements.
        QString dataType = "DoubleData";
        DataSpec req;
        QList<DataSpec> requirements;
        req.addStreamData(dataType);
        requirements.append(req);

        // Create the client.
        DirectStreamDataClient* client = static_cast<DirectStreamDataClient*>(
                clientFactory.create("DirectStreamDataClient"));
        CPPUNIT_ASSERT(DataClientFactory::whatIs(client) == "DirectStreamDataClient");
        client->reset( requirements );
        client->addStreamChunker("TestUdpChunker", "a");
        client->addStreamChunker("TestUdpChunker", "b");

        // Set up the data hash.
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(dataType, blobFactory.create(dataType));
#if 1
        for (int c = 0; c < numChunksToCheck; c++) {
            // Get the data.
            QHash<QString, DataBlob*> validData;
            validData = client->getData(dataHash);

            // Check the content of the data blob.
            DoubleData* data = (DoubleData*)validData.value("DoubleData");
            CPPUNIT_ASSERT(data);
            CPPUNIT_ASSERT(data->size() == 512/8);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2+(double)c, data->ptr()[0],
                    DBL_EPSILON);
            // TODO check data properly to make sure we have
            //      the right data
        }
#endif
        std::cout << __PRETTY_FUNCTION__ << " - DONE " << iter << std::endl;
        // FIXME what kills the DataReceiver here...
    } // iter loop
}


} // namespace pelican
