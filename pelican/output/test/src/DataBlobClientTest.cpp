#include "pelican/output/test/DataBlobClientTest.h"
#include "pelican/output/DataBlobClient.h"

#include "pelican/output/PelicanTCPBlobServer.h"
#include "pelican/output/Stream.h"
#include "pelican/data/test/TestDataBlob.h"
#include "pelican/utility/ConfigNode.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtTest/QSignalSpy>

namespace pelican {

using test::TestDataBlob;

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobClientTest );
/**
 * @details Constructs a DataBlobClientTest object.
 */
DataBlobClientTest::DataBlobClientTest()
    : CppUnit::TestFixture()
{
    qRegisterMetaType<Stream>("Stream");
}

/**
 * @details Destroys the DataBlobClientTest object.
 */
DataBlobClientTest::~DataBlobClientTest()
{
}

void DataBlobClientTest::setUp()
{
}

void DataBlobClientTest::tearDown()
{
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
        sleep(1);
        QCoreApplication::processEvents(); // give server chance to process subsciption request

        // Use Case:
        // server updates streams
        // expect:
        // updatedStreams signal to be emitted and streams() to return the
        // correct data
        QSignalSpy spy(client, SIGNAL( newStreamsAvailable()) );
        server->send( stream1, &blob);
        QCoreApplication::processEvents(); // server side events
        sleep(1);
        QCoreApplication::processEvents(); // process
        QSet<QString> streams = client->streams();
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
        CPPUNIT_ASSERT_EQUAL( 1, streams.size() );
        CPPUNIT_ASSERT( streams.contains(stream1) );

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

void DataBlobClientTest::test_subscribe()
{
    QString stream1("stream1");
    QString stream2("stream2");
    TestDataBlob blob1;
    blob1.setData("somedata");
    TestDataBlob blob2;
    blob1.setData("datablob2");
    {
        // Use Case:
        // subscribe() called on server that is down
        // Expect:
        // subscription to remain in force - keep trying until server comes up
        /*
        PelicanTCPBlobServer* server = _server();
        server->stop();
        DataBlobClient* client = _client(server);
        client->subscribe( stream1 );
        server->listen();
        QSignalSpy spy(client, SIGNAL( newData(const Stream&) ) );
        server->send(stream1,&blob1);
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
        delete client;
        delete server;
        */
    }
    {
        // Use Case:
        // subscribe() called on server with no known streams
        // Expect:
        // subscription to remain in force - no errors or messages
        PelicanTCPBlobServer* server = _server();
        DataBlobClient* client = _client(server);
        client->subscribe( stream1 );
        sleep(1);
        CPPUNIT_ASSERT_EQUAL( 1, server->clientsForStream( stream1 ) );
        delete client;
        delete server;
    }
    {
        // Use Case:
        // subscribe() called on server with stream
        // Expect:
        // to receive updates each time new data is sent
        PelicanTCPBlobServer* server = _server();
        server->send(stream1,&blob1); // ensure there is a stream on the server

        // set up the client and subscribe to a stream
        DataBlobClient* client = _client(server);
        CPPUNIT_ASSERT_EQUAL( 0, server->clientsForStream( stream1 ) );
        client->subscribe( stream1 );
        sleep(1);
        CPPUNIT_ASSERT_EQUAL( 1, server->clientsForStream( stream1 ) );

        // send some data and see if we get a copy
        QSignalSpy spy(client, SIGNAL( newData(const Stream&) ) );
        server->send(stream1,&blob1);
        QCoreApplication::processEvents();
        sleep(1);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
        server->send(stream1,&blob2);
        sleep(1);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL( 2, spy.count() );

        // Use Case:
        // send a stream to the server that we are not subscribed to
        // Expect: to receive no signal
        server->send("otherstream", &blob2);
        sleep(1);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL( 2, spy.count() );

        // Use Case:
        // subscribe() called for an additional stream
        // Expect: to be subscribed to both streams
        client->subscribe( stream2 );
        server->send(stream1, &blob1);
        sleep(1);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL( 3, spy.count() );
        server->send(stream2, &blob2);
        client->streams();
        sleep(1);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL( 4, spy.count() );

        // Use Case:
        // streams() called when already listening to stream data
        // Expect: no change in subscription
        QSet<QString> streams = client->streams();
        server->send(stream2, &blob1);
        sleep(1);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL( 3 , streams.size() );
        CPPUNIT_ASSERT( streams.contains(stream1) );
        CPPUNIT_ASSERT( streams.contains(stream2) );
        CPPUNIT_ASSERT( streams.contains("otherstream") );
        CPPUNIT_ASSERT_EQUAL( 5, spy.count() );

        delete client;
        delete server;
    }
}

DataBlobClient* DataBlobClientTest::_client( PelicanTCPBlobServer* server, const QString& xml )
{
    QString conf = xml;
    if( xml == "" )
        conf = QString("<connection host=\"127.0.0.1\" />");
    ConfigNode config(conf);
    DataBlobClient* client = new DataBlobClient(config);
    client->setPort(server->serverPort());
    client->setHost("127.0.0.1");
    //connect( client, SIGNAL( newStreamsAvailable() ), this, SLOT( _newStreamsAvailable() ));
    return client;
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
