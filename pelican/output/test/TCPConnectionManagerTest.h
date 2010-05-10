#ifndef TCPCONNECTIONMANAGERTEST_H
#define TCPCONNECTIONMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
class QTcpSocket;
class QCoreApplication;

/**
 * @file TCPConnectionManagerTest.h
 */

namespace pelican {
    class ServerRequest;
    class TCPConnectionManager;

/**
 * @class TCPConnectionManagerTest
 *  
 * @brief
 *    Unit test for the TCPConnectionManager
 * @details
 * 
 */

class TCPConnectionManagerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TCPConnectionManagerTest );
        CPPUNIT_TEST( test_brokenConnection );
        CPPUNIT_TEST( test_multiClients );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_brokenConnection();
        void test_multiClients();

    public:
        TCPConnectionManagerTest(  );
        ~TCPConnectionManagerTest();

    private:
        QTcpSocket* _createClient() const;
        void _sendRequest(QTcpSocket* tcpSocket, const ServerRequest& req);
        TCPConnectionManager* _server;
        QCoreApplication* _app;
};

} // namespace pelican
#endif // TCPCONNECTIONMANAGERTEST_H 
