#include "DataViewerTest.h"
#include "DataViewer.h"
#include <QtGui/QApplication>
#include "TestDataViewerWidget.h"

#include "pelican/output/DataBlobClient.h"
#include "pelican/output/Stream.h"
#include "pelican/data/test/TestDataBlob.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/memCheck.h"
#include "pelican/output/test/TestDataBlobClient.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataViewerTest );
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
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QApplication(argc,argv);
}

void DataViewerTest::tearDown()
{
    delete _app;
}

void DataViewerTest::test_client()
{
    QString testStream("testStream");
    TestDataBlob* blob = new TestDataBlob;
    blob->setData("somedata");
    // Use Case:
    // Add a simple DataBlobClient
    // and send some data
    //
    // Expect:
    // streams available to be updated
    // widgets update methods to be called
    //
    Config config;
    Config::TreeAddress address;
    address << Config::NodeId("viewers", "");
    ConfigNode configNode;
    TestDataBlobClient client;
    DataViewer viewer(config, address);
    viewer.setClient(client);
    viewer.setStreamViewer(testStream, "TestDataViewerWidget");
    viewer.enableStream(testStream);

    Stream str( testStream );
    str.setData(blob);
    client.send( str );
    _app->processEvents();
    CPPUNIT_ASSERT_EQUAL( 1, viewer.streams().size() );
    CPPUNIT_ASSERT_EQUAL( testStream.toStdString() , 
                          viewer.streams()[0].toStdString() );
    CPPUNIT_ASSERT_EQUAL( 1, TestDataViewerWidget::updateCalled() );

}

} // namespace pelican
