#include "server/test/PelicanServerTest.h"

#include "server/test/PelicanTestClient.h"
#include "server/PelicanServer.h"
#include "server/test/TestProtocol.h"
#include "server/DataManager.h"
#include "server/test/TestChunker.h"
#include "data/DataRequirements.h"
#include "utility/Config.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <iostream>

namespace pelican {

using test::TestProtocol;
using test::PelicanTestClient;

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanServerTest );

PelicanServerTest::PelicanServerTest() : CppUnit::TestFixture()
{
}

PelicanServerTest::~PelicanServerTest()
{
}

void PelicanServerTest::test_singleProtocolAcknowledge()
{
    // Use Case:
    // Attach to Server with a single protocol.
    // Expect response to the correct port
    try {
        Config config;
        PelicanServer server(&config);
        quint16 port = 3000;
        server.addProtocol(new TestProtocol(""), port);
        server.start();
        while (!server.isReady()) {}
        PelicanTestClient client(port);
        client.processAcknowledgement();
    }
    catch (const QString& e)
    {
        CPPUNIT_FAIL("Unexpected exception: " +  e.toStdString());
    }
}

void PelicanServerTest::test_singleProtocolStream()
{
    // Use Case:
    // Attach to Server with a single protocol.
    // Expect response to the correct port
    try {
        // Set the chunker configuration.
        Config config;
        QString serverXml = ""
                "<chunkers>"
                "    <TestChunker>"
                "       <data type=\"TestStream\"/>"
                "    </TestChunker>"
                "</chunkers>";
        config.setFromString("", serverXml);

        // Set up the server.
        PelicanServer server(&config);
        //server.setVerbosity(10000);
        quint16 port = 3000;
        server.addProtocol(new TestProtocol("", ServerRequest::StreamData), port);
        server.addStreamChunker("TestChunker");
        server.start();
        while (!server.isReady()) {}

        PelicanTestClient client(port);
        client.processStreamData();
    }
    catch (const QString& e)
    {
        CPPUNIT_FAIL("Unexpected exception: " +  e.toStdString());
    }
}

void PelicanServerTest::test_multiProtocol()
{
    QString id1("1");
    QString id2("2");

    // Use Case:
    // Assign multiple protocols to the same port
    // Expect an exception
    {
        Config config;
        PelicanServer server(&config);
        quint16 port = 30000;
        CPPUNIT_ASSERT_NO_THROW(server.addProtocol(new TestProtocol(id1), port));
        CPPUNIT_ASSERT_THROW(server.addProtocol(new TestProtocol(id2), port), QString);
    }

    // Use Case:
    // Assign multiple protocols to different ports
    // Expect no exceptions and client calls to be routed through
    // to the appropriate protocol object
    {
        try {
            Config config;
            PelicanServer server(&config);
            quint16 porta = 30000;
            quint16 portb = 30001;
            server.addProtocol(new TestProtocol(id1), porta);
            server.addProtocol(new TestProtocol(id2), portb);
            server.start();
            while (!server.isReady()) {}
            PelicanTestClient client1(porta);
            PelicanTestClient client2(portb);
            client1.processAcknowledgement();
            client2.processAcknowledgement();
        }
        catch (const QString& e) {
            CPPUNIT_FAIL("Unexpected exception: " +  e.toStdString());
        }
    }
}

} // namespace pelican
