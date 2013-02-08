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

#include "output/test/PelicanTCPBlobServerTest.h"
#include "output/PelicanTCPBlobServer.h"

#include "data/test/TestDataBlob.h"
#include "comms/PelicanClientProtocol.h"
#include "comms/StreamDataRequest.h"
#include "comms/PelicanProtocol.h"
#include "comms/StreamData.h"
#include "comms/ServerResponse.h"
#include "comms/DataBlobResponse.h"
#include "server/DataManager.h"
#include "comms/DataChunk.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

namespace pelican {

using test::TestDataBlob;

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanTCPBlobServerTest );

/**
 * @details
 * Constructor
 */
PelicanTCPBlobServerTest::PelicanTCPBlobServerTest()
    : CppUnit::TestFixture()
{ }

/**
 * @details
 * Desctructor
 */
PelicanTCPBlobServerTest::~PelicanTCPBlobServerTest()
{ }

/**
 * @details
 * Sets up environment/objects for test class
 */
void PelicanTCPBlobServerTest::setUp()
{
}

/**
 * @details
 * Destroys objects and reset environment
 */
void PelicanTCPBlobServerTest::tearDown()
{
}

/**
 * @details
 */
void PelicanTCPBlobServerTest::test_connection()
{
    // Use Case:
    // ?????? TODO
    // Expect: TODO
    //
    // Create and configure TCP server
    QString xml = "<PelicanTCPBlobServer>"
                  "   <connection port=\"0\"/>"  // 0 = find unused system port
                  "</PelicanTCPBlobServer>";
    ConfigNode config(xml);
    PelicanTCPBlobServer server(config);
    sleep(1);

    // Create a client and connect it to the server
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost( QHostAddress::LocalHost, server.serverPort() );
    if (!tcpSocket.waitForConnected(5000) || tcpSocket.state() == QAbstractSocket::UnconnectedState)
        CPPUNIT_FAIL("Client could not connect to server");

    // Define the data type which the client will except and send request
    StreamDataRequest req;
    DataSpec require;
    require.addStreamData("testData");
    req.addDataOption(require);

    PelicanClientProtocol clientProtocol;
    QByteArray data = clientProtocol.serialise(req);
    tcpSocket.write(data);
    tcpSocket.waitForBytesWritten(data.size());
    tcpSocket.flush();

    /// ensure we are registered before continuing
    while( server.clientsForStream("testData") == 0 )
    {
        sleep(1);
    }

    {
        // Test Server send
        TestDataBlob blob;
        blob.setData("Testing TCPServer");
        server.send("testData", &blob);

        // Evaluate the response from the server
        tcpSocket.waitForReadyRead();
        boost::shared_ptr<ServerResponse> r = clientProtocol.receive(tcpSocket);
        CPPUNIT_ASSERT( r->type() == ServerResponse::Blob );
        TestDataBlob blobResult;
        blobResult.deserialise(tcpSocket, ((DataBlobResponse*)r.get())->byteOrder());
        CPPUNIT_ASSERT(blobResult == blob);
    }

    {
        // Test Server send
        TestDataBlob blob;
        blob.setData("Testing TCPServer again");
        server.send("testData", &blob);

        // Evaluate the response from the server
        tcpSocket.waitForReadyRead();
        boost::shared_ptr<ServerResponse> r = clientProtocol.receive(tcpSocket);
        CPPUNIT_ASSERT( r->type() == ServerResponse::Blob );
        TestDataBlob blobResult;
        blobResult.deserialise(tcpSocket, ((DataBlobResponse*)r.get())->byteOrder());
        CPPUNIT_ASSERT(blobResult == blob);
    }
}

void PelicanTCPBlobServerTest::test_portConfig()
{
    QString xml = "<PelicanTCPBlobServer>"
                  "   <connection port=\"8899\"/>"  // 0 = find unused system port
                  "</PelicanTCPBlobServer>";
    ConfigNode config(xml);
    PelicanTCPBlobServer server(config);
    sleep(1);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)server.serverPort(),  (unsigned int)8899 );
}
} // namespace pelican
