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

#include "output/test/DataBlobClientTest.h"
#include "output/DataBlobClient.h"

#include "output/PelicanTCPBlobServer.h"
#include "output/Stream.h"
#include "data/test/TestDataBlob.h"
#include "utility/ConfigNode.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QString>
#include <QtCore/QtDebug>
#include <QtTest/QSignalSpy>

#include <unistd.h>

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

void DataBlobClientTest::test_streamInfo()
{
    QString stream1("stream1");
    TestDataBlob blob;
    blob.setData("somedata");

    try {
#if 0
        {
            // Use Case:
            //   Streams() called when there is no server (cannot connect)
            // Expect:
            //   Empty list returned after initial attempt
            DataBlobClient* client = _client();
            QSignalSpy spy(client, SIGNAL( newStreamsAvailable()) );
            CPPUNIT_ASSERT( client->streams() == QSet<QString>() );

            // Use Case:
            // Server comes online
            // expect updatedData to be called and streams to return the correct data
            PelicanTCPBlobServer* server = _server();
            server->send(stream1,&blob);
            CPPUNIT_ASSERT( client->streams().contains(stream1) );
            CPPUNIT_ASSERT_EQUAL( 1, spy.count() );
            delete client;
            delete server;
        }
#endif
        {
            // Use Case:
            //   streams() called on server with no known streams
            // Expect:
            //   empty list
            PelicanTCPBlobServer* server = _server();
            DataBlobClient* client = _client(server);
            CPPUNIT_ASSERT(client->streams() == QSet<QString>());
            usleep(10);
            QCoreApplication::processEvents(); // give server chance to process subsciption request

            // Use Case:
            //   Server updates streams
            // Expect:
            //   UpdatedStreams signal to be emitted and streams() to return the
            //   correct data
            QSignalSpy spy(client, SIGNAL(newStreamsAvailable()));
            server->send(stream1, &blob);
            usleep(100);
            QCoreApplication::processEvents(); // server side events
            QSet<QString> streams = client->streams();
            CPPUNIT_ASSERT_EQUAL(1, spy.count());
            CPPUNIT_ASSERT_EQUAL(1, streams.size());
            CPPUNIT_ASSERT(streams.contains(stream1));

            delete client;
            delete server;
        }

        {
            // Use Case:
            //   Streams() called on server with streams
            // Expect:
            //   Correct data to be returned by streams, no signal need be emitted
            PelicanTCPBlobServer* server = _server();
            server->send(stream1, &blob); // ensure there is a stream on the server
            DataBlobClient* client = _client(server);
            QSignalSpy spy(client, SIGNAL(newStreamsAvailable()));
            CPPUNIT_ASSERT_EQUAL(0, spy.count());
            delete client;
            delete server;
        }

    }
    catch (const QString& msg) {
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
        client->subscribe(stream1);
        usleep(200);
        CPPUNIT_ASSERT_EQUAL(1, server->clientsForStream(stream1));
        delete client;
        delete server;
    }
    {
        // Use Case:
        // subscribe() called on server with stream
        // Expect:
        // to receive updates each time new data is sent
        PelicanTCPBlobServer* server = _server();
        server->send(stream1, &blob1); // ensure there is a stream on the server

        // set up the client and subscribe to a stream
        DataBlobClient* client = _client(server);
        CPPUNIT_ASSERT_EQUAL(0, server->clientsForStream(stream1));
        client->subscribe(stream1);
        usleep(200);
        CPPUNIT_ASSERT_EQUAL(1, server->clientsForStream(stream1));

        // send some data and see if we get a copy
        QSignalSpy spy(client, SIGNAL( newData(const Stream&) ) );
        server->send(stream1, &blob1);
        QCoreApplication::processEvents();
        usleep(10);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL(1, spy.count());
        server->send(stream1, &blob2);
        usleep(100);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL(2, spy.count());

        // Use Case:
        // send a stream to the server that we are not subscribed to
        // Expect: to receive no signal
        server->send("otherstream", &blob2);
        usleep(10);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL(2, spy.count());

        // Use Case:
        // subscribe() called for an additional stream
        // Expect: to be subscribed to both streams
        client->subscribe(stream2);
        server->send(stream1, &blob1);
        usleep(10);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL(3, spy.count());
        server->send(stream2, &blob2);
        client->streams();
        usleep(10);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL(4, spy.count());

        // Use Case:
        // streams() called when already listening to stream data
        // Expect: no change in subscription
        QSet<QString> streams = client->streams();
        server->send(stream2, &blob1);
        usleep(10);
        QCoreApplication::processEvents();
        CPPUNIT_ASSERT_EQUAL(3 , streams.size());
        CPPUNIT_ASSERT(streams.contains(stream1));
        CPPUNIT_ASSERT(streams.contains(stream2));
        CPPUNIT_ASSERT(streams.contains("otherstream"));
        CPPUNIT_ASSERT_EQUAL(5, spy.count());

        delete client;
        delete server;
    }
}


DataBlobClient* DataBlobClientTest::_client(PelicanTCPBlobServer* server,
        const QString& xml)
{
    QString conf = xml;
    if (xml == "")
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
    QString xml = QString(
            "<PelicanTCPBlobServer>"
            "   <connection port=\"%1\"/>"
            "</PelicanTCPBlobServer>"
    ).arg((unsigned int)port);
    ConfigNode config(xml);
    return new PelicanTCPBlobServer(config);
}

} // namespace pelican
