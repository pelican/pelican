#include "SocketTesterTest.h"
#include "SocketTester.h"
#include <QByteArray>
#include <QCoreApplication>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( SocketTesterTest );
// class SocketTesterTest 
SocketTesterTest::SocketTesterTest()
    : CppUnit::TestFixture()
{
}

SocketTesterTest::~SocketTesterTest()
{
}

void SocketTesterTest::setUp()
{
    int argc = 0;
    char* argv[1];
    _app = new QCoreApplication(argc,argv);
}

void SocketTesterTest::tearDown()
{
    delete _app;
}

void SocketTesterTest::test_method()
{
    SocketTester st;
    QByteArray b("test");
    QTcpSocket& sock = st.send(b);
    CPPUNIT_ASSERT( sock.isReadable() );
    sock.waitForReadyRead(2000);
    CPPUNIT_ASSERT_EQUAL( b.size(), (int)sock.bytesAvailable() );
    CPPUNIT_ASSERT( sock.readAll() == b );

}

} // namespace pelican
