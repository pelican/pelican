#ifndef TCPCONNECTIONMANAGERTEST_H
#define TCPCONNECTIONMANAGERTEST_H

/**
 * @file TCPConnectionManagerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
class QTcpSocket;

namespace pelican {
class ServerRequest;
class TCPConnectionManager;
class PelicanClientProtocol;

/**
 * @ingroup t_output
 *
 * @class TCPConnectionManagerTest
 *
 * @brief
 * Unit test for the TCPConnectionManager
 *
 * @details
 */

class TCPConnectionManagerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TCPConnectionManagerTest );
        CPPUNIT_TEST( test_send );
        CPPUNIT_TEST( test_brokenConnection );
        CPPUNIT_TEST( test_multiClients );
        CPPUNIT_TEST( test_dataSupportedRequest );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_send();
        void test_brokenConnection();
        void test_multiClients();
        void test_dataSupportedRequest();

    public:
        TCPConnectionManagerTest(  );
        ~TCPConnectionManagerTest();

    private:
        QTcpSocket* _createClient() const;
        void _sendRequest(QTcpSocket* tcpSocket, const ServerRequest& req);
        TCPConnectionManager* _server;
        PelicanClientProtocol* _clientProtocol;
};

} // namespace pelican
#endif // TCPCONNECTIONMANAGERTEST_H
