#include "DataViewerTest.h"
#include "DataViewer.h"
#include "TestDataViewerWidget.h"

#include "pelican/output/DataBlobClient.h"
#include "pelican/output/PelicanTCPBlobServer.h"
#include "pelican/output/Stream.h"
#include "pelican/data/test/TestDataBlob.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/output/test/TestDataBlobClient.h"

#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <boost/shared_ptr.hpp>

#include <iostream>

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

void DataViewerTest::setUp()
{

}

void DataViewerTest::tearDown()
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
    sleep(1);

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
