#include <QString>
#include <QCoreApplication>
#include "test/PelicanTCPBlobServerTest.h"
#include "PelicanTCPBlobServer.h"

#include "pelican/data/test/TestDataBlob.h"
#include "pelican/comms/PelicanClientProtocol.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/comms/StreamData.h"
#include "pelican/server/DataManager.h"
#include "pelican/utility/memCheck.h"
#include "pelican/comms/Data.h"

namespace pelican {

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
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc,argv);
}

/**
 * @details
 * Destroys objects and reset environment
 */
void PelicanTCPBlobServerTest::tearDown()
{ 
    delete _app;
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
    QString xml = "<TCPBlob>"
                  "   <connection port=\"0\"/>"  // 0 = find unused system port
                  "</TCPBlob>";
    ConfigNode config(xml);
    PelicanTCPBlobServer server(config, _app);
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

    // Test Server send
    TestDataBlob blob;
    blob.setData("Testing TCPServer");
    server.send("testData", blob);
}

} // namespace pelican
