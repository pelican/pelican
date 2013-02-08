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

#include "PelicanServerClientTest.h"
#include "PelicanServerClient.h"
#include "data/DataSpec.h"

#include "data/test/TestDataBlob.h"
#include "core/test/TestServiceAdapter.h"
#include "core/test/TestStreamAdapter.h"

#include "utility/Config.h"
#include "utility/ConfigNode.h"
#include "utility/test/SocketTester.h"
#include "comms/StreamData.h"
#include "comms/DataChunk.h"
#include "comms/StreamDataResponse.h"
#include "comms/ServiceDataResponse.h"
#include "server/test/TestServer.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtXml/QDomElement>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpSocket>

namespace pelican {

using test::TestDataBlob;
using test::SocketTester;
using test::TestServiceAdapter;
using test::TestStreamAdapter;

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanServerClientTest );

/**
 *@details PelicanServerClientTest
 */
PelicanServerClientTest::PelicanServerClientTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
PelicanServerClientTest::~PelicanServerClientTest()
{
}

void PelicanServerClientTest::setUp()
{
}

void PelicanServerClientTest::tearDown()
{
}

void PelicanServerClientTest::test_getData()
{
    Config config;
    config.setFromString(
            "<testconfig>"
            "   <server host=\"127.0.0.1\"/>"
            "</testconfig>"
            );
    Config::TreeAddress address;
    address << Config::NodeId("testconfig", "");
    ConfigNode configNode = config.get(address);
    TestStreamAdapter streamAdapter;
    QString stream1("stream1");
    QString version1("version1");
    DataSpec req;
    req.addStreamData(stream1);
    {
        // Use Case:
        //    data requirements are set, empty hash passed
        // Expect: throw

        QList<DataSpec> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.setAdapter(stream1,&streamAdapter);
        dt.addData(lreq);
        PelicanServerClient client(configNode, dt, 0);
        QHash<QString, DataBlob*> dataHash;
        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString );
    }
    {
        // Use Case:
        //    Data Requirements do not match DataBlobs
        // Expect:
        //     throw with a suitable message

        // setup the test
        QList<DataSpec> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.setAdapter(stream1,&streamAdapter);
        dt.addData(lreq);
        PelicanServerClient client(configNode, dt, 0);

        QHash<QString, DataBlob*> dataHash;
        DataBlob db("DataBlob");
        dataHash.insert("data2", &db);

        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString);
    }
}

void PelicanServerClientTest::test_response()
{
    try {
    TestServiceAdapter serviceAdapter;
    TestStreamAdapter streamAdapter;
    ConfigNode configNode;
    configNode.setFromString(
            "<testconfig>"
            "   <server host=\"127.0.0.1\"/>"
            "</testconfig>"
    );
    QString stream1("stream1");
    QString version1("v1");
    QString service1("service1");
    QString serviceVersion1("sv1");
    DataSpec reqStream1;
    reqStream1.addStreamData(stream1);
    QByteArray data1("data1");
    {
        // Use Case
        // receive a StreamData response (single stream, no service data)
        boost::shared_ptr<ServerResponse> res( new StreamDataResponse );
        static_cast<StreamDataResponse*>(res.get())->setStreamData( new StreamData(stream1,version1, data1.size()) );

        // set up the adapters and client
        QList<DataSpec> lreq;
        lreq.append(reqStream1);
        DataTypes dt;
        dt.setAdapter(stream1,&streamAdapter);
        dt.addData(lreq);
        PelicanServerClient client(configNode, dt, 0);

        // set up the memory for receiving data
        TestDataBlob db;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(stream1, &db);

        SocketTester st;
        QTcpSocket& sock = st.send(data1);
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), vhash[stream1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data1.data() ) , std::string( static_cast<TestDataBlob*>(vhash[stream1])->data() ) );
    }
    QByteArray data2("data2");
    {
        // Use Case
        // ServiceData response - single service data
        boost::shared_ptr<ServerResponse> res( new ServiceDataResponse );
        static_cast<ServiceDataResponse*>(res.get())->addData( new DataChunk(service1,serviceVersion1, data2.size()) );

        // set up the memory for receiving data
        TestDataBlob db;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(service1, &db);

        // set up the adapters and client
        DataSpec req;
        req.addStreamData(service1);
        QList<DataSpec> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.setAdapter(service1,&serviceAdapter);
        dt.addData(lreq);
        PelicanServerClient client(configNode, dt, 0);

        SocketTester st;
        QTcpSocket& sock = st.send(data2);
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( serviceVersion1.toStdString(), vhash[service1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data2.data() ) , std::string( static_cast<TestDataBlob*>(vhash[service1])->data() ) );
    }
    QString service2("service2");
    {
        // Use Case
        // ServiceData response - multiple service data
        boost::shared_ptr<ServerResponse> res( new ServiceDataResponse );
        static_cast<ServiceDataResponse*>(res.get())->addData( new DataChunk(service1,version1, data1.size()) );
        static_cast<ServiceDataResponse*>(res.get())->addData( new DataChunk(service2,serviceVersion1, data2.size()) );

        // set up the memory for receiving data
        TestDataBlob db;
        TestDataBlob db2;
        QHash<QString, DataBlob*> dataHash;
        dataHash.insert(service1, &db);
        dataHash.insert(service2, &db2);

        // set up the adapters and client
        DataSpec req;
        req.addStreamData(service1);
        QList<DataSpec> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.setAdapter(service1,&serviceAdapter);
        dt.setAdapter(service2,&serviceAdapter);
        dt.addData(lreq);
        PelicanServerClient client(configNode, dt, 0);

        SocketTester st;
        QTcpSocket& sock = st.send( data1 + data2 );
        QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
        CPPUNIT_ASSERT( vhash == dataHash );
        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), vhash[service1]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( serviceVersion1.toStdString(), vhash[service2]->version().toStdString() );
        CPPUNIT_ASSERT_EQUAL( std::string( data1.data() ) , std::string( static_cast<TestDataBlob*>(vhash[service1])->data() ) );
        CPPUNIT_ASSERT_EQUAL( std::string( data2.data() ) , std::string( static_cast<TestDataBlob*>(vhash[service2])->data() ) );
    }

//    {
//        // Use Case
//        // receive a StreamData response with associated service data (single stream, single service data)
//        // no existing service data exists
//        boost::shared_ptr<ServerResponse> res( new StreamDataResponse );
//        StreamData* sd = new StreamData(stream1,version1, data1.size());
//        sd->addAssociatedData(boost::shared_ptr<Data>(new Data(service1,serviceVersion1, data2.size()) ) );
//        static_cast<StreamDataResponse*>(res.get())->setStreamData( sd );
//
//        // set up the adapters and client
//        DataSpec req;
//        req.addStreamData(stream1);
//        req.addServiceData(service1);
//        QList<DataSpec> lreq;
//        lreq.append(req);
//        DataTypes dt;
//        dt.setAdapter(stream1,&streamAdapter);
//        dt.setAdapter(service1,&serviceAdapter);
//        dt.addData(lreq);
//        PelicanServerClient client(configNode, dt, 0);
//
//        // set up the memory for receiving data
//        TestDataBlob db;
//        TestDataBlob db2;
//        QHash<QString, DataBlob*> dataHash;
//        dataHash.insert(stream1, &db);
//        dataHash.insert(service1, &db2);
//
//        SocketTester st;
//        QTcpSocket& sock = st.send(data1);
//        try {
//            QHash<QString, DataBlob*> vhash = client._response( sock , res, dataHash );
//        } catch (const QString& e) {
//            CPPUNIT_FAIL(e.toStdString());
//        }
//        CPPUNIT_ASSERT( vhash == dataHash );
//        CPPUNIT_ASSERT_EQUAL( version1.toStdString(), vhash[stream1]->version().toStdString() );
//        CPPUNIT_ASSERT_EQUAL( serviceVersion1.toStdString(), vhash[service1]->version().toStdString() );
//        CPPUNIT_ASSERT_EQUAL( std::string( data1.data() ) , std::string( static_cast<TestDataBlob*>( vhash[stream1])->data() ) );
//        CPPUNIT_ASSERT_EQUAL( std::string( data2.data() ) , std::string( static_cast<TestDataBlob*>( vhash[service1])->data() ) );
//    }

        } catch ( const QString& e ) {
                CPPUNIT_FAIL( e.toStdString() );
        }
}

} // namespace pelican
