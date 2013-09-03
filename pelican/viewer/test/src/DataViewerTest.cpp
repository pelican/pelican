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

#include "DataViewerTest.h"
#include "DataViewer.h"
#include "TestDataViewerWidget.h"

#include "output/DataBlobClient.h"
#include "output/PelicanTCPBlobServer.h"
#include "output/Stream.h"
#include "data/test/TestDataBlob.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"
#include "output/test/TestDataBlobClient.h"

#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <unistd.h>

using namespace std;

namespace pelican {

using test::TestDataBlob;
using test::TestDataBlobClient;
using test::TestDataViewerWidget;

/**
 * @details Constructs a DataViewerTest object.
 */
DataViewerTest::DataViewerTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the DataViewerTest object.
 */
DataViewerTest::~DataViewerTest()
{
}

void DataViewerTest::test_client()
{
    QString testStream("testStream");
    boost::shared_ptr<TestDataBlob> blob( new TestDataBlob );
    blob->setData("somedata");

    // Use Case:
    // Add a simple DataBlobClient
    // and send some data
    //
    // Expect:
    // streams available to be updated
    // widgets update methods to be called
    //
    {
        Config config;
        Config::TreeAddress address;
        address << Config::NodeId("viewers", "");

        ConfigNode configNode;
        TestDataBlobClient client;
        DataViewer viewer(config, address);
        viewer.setClient(client);
        viewer.setStreamViewer(testStream, "TestDataViewerWidget");
        CPPUNIT_ASSERT_EQUAL(0, client.subscriptions().size() );
        viewer.enableStream(testStream);

        Stream str( testStream );
        str.setData(blob);
        client.send( str );
        QCoreApplication::processEvents();

        CPPUNIT_ASSERT_EQUAL( 1, viewer.streams().size() );
        CPPUNIT_ASSERT_EQUAL( testStream.toStdString() ,
                viewer.streams()[0].toStdString() );
        CPPUNIT_ASSERT_EQUAL(1, client.subscriptions().size() );
        CPPUNIT_ASSERT_EQUAL( 1, TestDataViewerWidget::updateCalled() );
    }
}

void DataViewerTest::test_integrationWithDataClient()
{
    // Use Case:
    // full integration test with a rela DataBlobClient
    // Setup a test server
    QString xml = "<PelicanTCPBlobServer>"
        "   <connection port=\"0\"/>"  // 0 = find unused system port
        "</PelicanTCPBlobServer>";

    ConfigNode config(xml);
    PelicanTCPBlobServer server(config);
    usleep(100);

    DataBlobClient* client = _client( &server );
//    DataViewer viewer(config, address);
//    viewer.setClient(client);
    TestDataBlob blob;
    blob.setData("integration test");
    server.send("testData", &blob);

    delete client;
}


DataBlobClient* DataViewerTest::_client( PelicanTCPBlobServer* server )
{
    QString conf = QString("<connection host=\"127.0.0.1\" />");
    ConfigNode config(conf);
    DataBlobClient* client = new DataBlobClient(config);
    client->setPort(server->serverPort());
    client->setHost("127.0.0.1");
    //connect( client, SIGNAL( newStreamsAvailable() ), this, SLOT( _newStreamsAvailable() ));
    return client;
}

} // namespace pelican
