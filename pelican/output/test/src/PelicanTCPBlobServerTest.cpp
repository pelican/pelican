#include "test/PelicanTCPBlobServerTest.h"
#include "PelicanTCPBlobServer.h"

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
void PelicanTCPBlobServerTest::test_method()
{
    PelicanTCPBlobServer server;
    
    // create a client and try to connect to server
    QHostAddress address("127.0.0.1");
    QTcpSocket tcpSocket;

    tcpSocket.connectToHost(address, 8888);

    if (!tcpSocket.waitForConnected(5000) || tcpSocket.state() == QAbstractSocket::UnconnectedState)
        printf("Socket could not connect\n");
}

} // namespace pelican
