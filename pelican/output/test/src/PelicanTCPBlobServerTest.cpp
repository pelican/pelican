#include "pelican/output/test/PelicanTCPBlobServerTest.h"
#include "pelican/output/PelicanTCPBlobServer.h"

#include "pelican/data/test/TestDataBlob.h"
#include "pelican/comms/PelicanClientProtocol.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/ServerResponse.h"
#include "pelican/comms/DataBlobResponse.h"
#include "pelican/server/DataManager.h"
#include "pelican/comms/DataChunk.h"

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
    DataRequirements require;
    require.setStreamData("testData");
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
