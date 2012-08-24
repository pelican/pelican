#include "DataBlobChunkerTest.h"
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpSocket>
#include <QtTest/QSignalSpy>
#include "pelican/server/test/ChunkerTester.h"
#include "pelican/output/test/TestDataBlobServer.h"
#include <QtCore/QDebug>


namespace pelican {
using namespace test;

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobChunkerTest );
/**
 *@details DataBlobChunkerTest 
 */
DataBlobChunkerTest::DataBlobChunkerTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataBlobChunkerTest::~DataBlobChunkerTest()
{
}

void DataBlobChunkerTest::setUp()
{
    _blob.setData( QByteArray("testdata") );
}

void DataBlobChunkerTest::tearDown()
{
}

void DataBlobChunkerTest::test_method()
{
    // Use Case:
    // Send datablobs on a single stream
    // Expect:
    // individual requests for buffer space 
    // for each DataBlob
    try {
    TestDataBlobServer server;
    QString xml("<DataBlobChunker>");
    xml += QString("<connection host=\"%1\" port=\"%2\" />")
           .arg(server.host()).arg(server.port());
    xml += QString("<subscribe stream=\"%1\" />").arg(_blob.type());
    xml += "</DataBlobChunker>";
    CPPUNIT_ASSERT_EQUAL( 0, server.clientsForStream(_blob.type()) );
    ChunkerTester tester("DataBlobChunker", 10*_blob.size(), xml );
    CPPUNIT_ASSERT_EQUAL( 1, server.clientsForStream(_blob.type()) );
    server.send(&_blob);
    CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount() );
    server.send(&_blob);
    CPPUNIT_ASSERT_EQUAL( 2, tester.writeRequestCount() );
    } catch( QString e ) {
        CPPUNIT_FAIL(e.toStdString().c_str());
    }
}

void DataBlobChunkerTest::test_multisubscribe() {
    // Use Case:
    // Send datablobs on multiple streams
    // Expect:
    // individual requests for buffer space 
    // for each DataBlob - different buffers for
    // each stream
    TestDataBlobServer server;
    QString stream1("stream1");
    QString stream2("stream2");
    QString xml("<DataBlobChunker>");
    xml += QString("<connection host=\"%1\" port=\"%2\" />")
           .arg(server.host()).arg(server.port());
    xml += QString("<subscribe stream=\"%1\" />").arg(stream1);
    xml += QString("<subscribe stream=\"%1\" />").arg(stream2);
    xml += "</DataBlobChunker>";
    QList<QString> list; list << stream1 << stream2;
    ChunkerTester tester("DataBlobChunker", 10*_blob.size(), xml );
    CPPUNIT_ASSERT_EQUAL( 1, server.clientsForStream(stream1) );
    CPPUNIT_ASSERT_EQUAL( 1, server.clientsForStream(stream2) );
    server.send(&_blob, stream1);
    CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount(stream1) );
    server.send(&_blob, stream2);
    CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount(stream2) );
}

} // namespace pelican
