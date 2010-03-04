#ifndef PELICANPROTOCOLTEST_H
#define PELICANPROTOCOLTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "PelicanClientProtocol.h"
class QTcpSocket;

/**
 * @file PelicanProtocolTest.h
 */

namespace pelican {

class SocketTester;
class ServerRequest;

/**
 * @class PelicanProtocolTest
 *  
 * @brief
 *  Unit test for testing the Pelican server protocol
 * @details
 * 
 */

class PelicanProtocolTest : public CppUnit::TestFixture
{
    protected:
        typedef QTcpSocket Socket_t;
        Socket_t& _send(ServerRequest*);

    public:
        CPPUNIT_TEST_SUITE( PelicanProtocolTest );
        CPPUNIT_TEST( test_request );
        CPPUNIT_TEST( test_sendStreamData );
        CPPUNIT_TEST( test_sendServiceData );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_request();
        void test_sendStreamData();
        void test_sendServiceData();

    public:
        PelicanProtocolTest(  );
        ~PelicanProtocolTest();
        SocketTester* _st;
        PelicanClientProtocol _protocol;

};

} // namespace pelican
#endif // PELICANPROTOCOLTEST_H 
