#include <QString>
#include <QCoreApplication>
#include "test/PelicanTCPBlobServerTest.h"
#include "PelicanTCPBlobServer.h"

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
void PelicanTCPBlobServerTest::test_method()
{
    // Create and configure TCP server
    QString xml = "<TCPBlob>"
                  "   <connection port=\"8888\"/>"
                  "</TCPBlob>";
    ConfigNode config(xml);
    PelicanTCPBlobServer server(config, _app);
    sleep(1);
    printf("Created server\n");
    
    // Create a client
    QHostAddress address("127.0.0.1");
    QTcpSocket tcpSocket;

    // Connected to server
    tcpSocket.connectToHost(address, 8888);
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

    // Make sure client data structure has been set up properly
    sleep(1);
    CPPUNIT_ASSERT_EQUAL(server._clients -> keys().count(), 1);
    CPPUNIT_ASSERT(server._clients -> keys()[0] == QString("testData"));
    CPPUNIT_ASSERT_EQUAL((*server._clients)["testData"].count(), 1);

    // Test Server send
    QByteArray serverData("Testing TCPServer");
    server.send("testData", serverData);
}

} // namespace pelican
