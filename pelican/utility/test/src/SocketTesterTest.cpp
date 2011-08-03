#include "pelican/utility/test/SocketTesterTest.h"
#include "pelican/utility/test/SocketTester.h"

#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>

namespace pelican {

using test::SocketTester;

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
