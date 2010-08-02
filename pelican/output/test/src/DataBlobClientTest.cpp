#include "DataBlobClientTest.h"
#include "DataBlobClient.h"
#include <QSet>
#include <QString>
#include <QtTest/QSignalSpy>

#include "pelican/data/test/TestDataBlob.h"
#include "pelican/output/PelicanTCPBlobServer.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobClientTest );
/**
 * @details Constructs a DataBlobClientTest object.
 */
DataBlobClientTest::DataBlobClientTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the DataBlobClientTest object.
 */
DataBlobClientTest::~DataBlobClientTest()
{
}

void DataBlobClientTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc,argv);
}

void DataBlobClientTest::tearDown()
{
    delete _app;
}

void DataBlobClientTest::test_streamInfo()
{
    QString stream1("stream1");
    TestDataBlob blob;
    blob.setData("somedata");
    try {
    {
/*
        // Use Case: TODO
        // streams() called when there is no server (cannot connect)
        // Expect:
        // empty list returned after initial attempt
        DataBlobClient* client = _client();
        QSignalSpy spy(client, SIGNAL( newStreamsAvailable()) );
        CPPUNIT_ASSERT( client->streams() == QSet<QString>() );

        // Use Case:
        // Server comes online
        // expect updatedData to be called and streams to return the correct data
        PelicanTCPBlobServer* server = _server();
        server->send(stream1,&blob);
        CPPUNIT_ASSERT( client->streams().contains(stream1) );
        CPPUNIT_ASSERT( 1, spy.count() );
        delete client;
        delete server;
*/
    }
    {
        PelicanTCPBlobServer* server = _server();
        // Use Case:
        // streams() called on server with no known streams
        // Expect: 
        // empty list
        DataBlobClient* client = _client(server);
        CPPUNIT_ASSERT( client->streams() == QSet<QString>() );

        // Use Case: 
        // server updates streams
        // expect:
        // updatedStreams signal to be emitted and streams() to return the
        // correct data
        QSignalSpy spy(client, SIGNAL( newStreamsAvailable()) );
        server->send(stream1,&blob);
        _app->processEvents();
        CPPUNIT_ASSERT( client->streams().contains(stream1) );
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );

        delete client;
        delete server;
    }
    {
        // Use Case:
        // streams() called on server with streams
        // Expect: 
        // correct data to be returned by streams, no signal need be emitted
        PelicanTCPBlobServer* server = _server();
        server->send(stream1,&blob); // ensure there is a stream on the server

        DataBlobClient* client = _client(server);
        QSignalSpy spy(client, SIGNAL( newStreamsAvailable()) );
        CPPUNIT_ASSERT_EQUAL( 0, spy.count() );

        delete client;
        delete server;
    }
    }
    catch( const QString msg ) 
    {
        CPPUNIT_FAIL(msg.toStdString());
    }
}

DataBlobClient* DataBlobClientTest::_client( PelicanTCPBlobServer* server, const QString& xml )
{
    QString conf = xml;
    if( xml == "" )
        conf = QString("<connection port=\"%1\" server=\"127.0.0.1\" />").arg(server->serverPort());
    ConfigNode config(conf);
    return new DataBlobClient(config);
}


PelicanTCPBlobServer* DataBlobClientTest::_server(quint16 port)
{
    QString xml = QString(  "<PelicanTCPBlobServer>"
                            "   <connection port=\"%1\"/>"  
                            "</PelicanTCPBlobServer>" ).arg((unsigned int)port);
    ConfigNode config(xml);
    return new PelicanTCPBlobServer(config);
}
} // namespace pelican
