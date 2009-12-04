#include "SessionTest.h"
#include "Session.h"
#include "utility/ServerRequest.h"
#include "TestProtocol.h"
#include <QByteArray>
#include <QDataStream>
#include <QCoreApplication>
#include <iostream>


#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( SessionTest );
// class SessionTest 
SessionTest::SessionTest()
    : CppUnit::TestFixture()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

SessionTest::~SessionTest()
{
    delete _app;
}

void SessionTest::setUp()
{
}

void SessionTest::tearDown()
{
}

void SessionTest::test_processRequest()
{
    QString id = "1";
    TestProtocol proto(id);
    Session session(0, &proto);
    {
        // Use Case:
        // Request an acknowledgement
        // Expect the id to be returned
        ServerRequest request("ACK");

        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        CPPUNIT_ASSERT_EQUAL( 0, block.size() );
        session.processRequest(request, out);
        CPPUNIT_ASSERT( block.size() != 0 );
        
    }
}

} // namespace pelican
