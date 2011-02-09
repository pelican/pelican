#ifndef PELICANPROTOCOLTEST_H
#define PELICANPROTOCOLTEST_H

/**
 * @file PelicanProtocolTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

#include "pelican/comms/PelicanClientProtocol.h"

class QTcpSocket;

namespace pelican {

namespace test {
class SocketTester;
}
class ServerRequest;

/**
 * @ingroup t_comms
 *
 * @class PelicanProtocolTest
 *
 * @brief
 * Unit test for testing the Pelican server protocol.
 *
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
        CPPUNIT_TEST( test_sendDataBlob );
        CPPUNIT_TEST( test_sendDataSupport );
        CPPUNIT_TEST( test_sendChunk );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_request();
        void test_sendStreamData();
        void test_sendServiceData();
        void test_sendDataBlob();
        void test_sendDataSupport();
        void test_sendChunk();

    public:
        PelicanProtocolTest();
        ~PelicanProtocolTest();
        test::SocketTester* _st;
        PelicanClientProtocol _protocol;
};

} // namespace pelican
#endif // PELICANPROTOCOLTEST_H
