#include "OutputStreamManagerTest.h"
#include "OutputStreamManager.h"

#include "pelican/output/test/TestOutputStreamer.h"

namespace pelican {

using test::TestOutputStreamer;

CPPUNIT_TEST_SUITE_REGISTRATION( OutputStreamManagerTest );
/**
 *@details OutputStreamManagerTest
 */
OutputStreamManagerTest::OutputStreamManagerTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
OutputStreamManagerTest::~OutputStreamManagerTest()
{
}

void OutputStreamManagerTest::setUp()
{
    _config = new Config();
}

void OutputStreamManagerTest::tearDown()
{
    delete _config;
}

void OutputStreamManagerTest::test_connectToStream()
{
    QString all("all");
    QString stream1("stream1");
    QString stream2("stream2");
    TestOutputStreamer s1("Streamer1");
    TestOutputStreamer s2("Streamer2");
    {
        // Use Case:
        //     Simple one-one association
        OutputStreamManager* os = _getManager();
        os->connectToStream( &s1, stream1 );
        CPPUNIT_ASSERT(os->connected(stream1).size() == 1 );
        CPPUNIT_ASSERT(os->connected(stream1)[0] == &s1 );
        delete os;
    }
    {
        // Use Case:
        //     "all" association followed by an association adding
        //     a new stream.
        // Expect:
        //     "all" stream to be connected to the new stream
        //     and correct association for new stream
        OutputStreamManager* os = _getManager();
        os->connectToStream( &s1, all );
        os->connectToStream( &s2, stream1 );
        CPPUNIT_ASSERT(os->connected(stream1)[0] == &s1 );
        CPPUNIT_ASSERT(os->connected(stream1)[1] == &s2 );
        delete os;
    }
    {
        // Use Case:
        //     "all" association following by an existing association
        //
        // Expect:
        //     exisiting associations stream to be connected to the "all" stream
        //     and exisiting connections maintained
        OutputStreamManager* os = _getManager();
        os->connectToStream( &s1, stream1 );
        os->connectToStream( &s2, all );
        CPPUNIT_ASSERT(os->connected(stream1)[0] == &s1 );
        CPPUNIT_ASSERT(os->connected(stream1)[1] == &s2 );
        delete os;
    }
}

void OutputStreamManagerTest::test_configuration()
{
    {
        // Use Case:
        //     configuration contains a dataStreams, and a known Output Streamers defined
        // Expect:
        //     throw
        QString xml =
            "<output>"
            "   <streamers>"
            "     <TestOutputStreamer />"
            "   </streamers>"
            "   <dataStreams>"
            "      <stream name=\"a\" listeners=\"TestOutputStreamer\" />"
            "   </dataStreams>"
            "</output>"
            ;
        OutputStreamManager* os = 0;
        try {
            os = _getManager(xml, "output");
            CPPUNIT_ASSERT(os->connected("a").size() == 1 );
        }
        catch( const QString& msg ) {
            CPPUNIT_FAIL( msg.toStdString() );
        }
        delete os;
    }
    {
        // Use Case:
        //     configuration contains an unknown output streamer type
        // Expect:
        //     throw
        QString xml =
            "<output>"
            "   <streamers>"
            "     <UnknownStreamer />"
            "   </streamers>"
            "   <dataStreams>"
            "      <stream name=\"a\" listeners=\"UnknownStreamer\" />"
            "   </dataStreams>"
            "</output>"
            ;
        CPPUNIT_ASSERT_THROW( _getManager(xml, "output"), QString );
    }
    {
        // Use Case:
        //     configuration contains no dataStreams, but output Streamers defined
        // Expect:
        //     throw
        QString xml =
            "<output>"
            "   <streamers>"
            "     <TestOutputStreamer />"
            "   </streamers>"
            "</output>"
            ;
        CPPUNIT_ASSERT_THROW( _getManager(xml, "output") , QString );
    }
    {
        // Use Case:
        //     configuration contains multiple output streamers of the same type
        //     and the same identifier
        // Expect:
        //     throw
        QString xml =
            "<output>"
            "   <streamers>"
            "     <TestOutputStreamer />"
            "     <TestOutputStreamer />"
            "   </streamers>"
            "   <dataStreams>"
            "      <stream name=\"a\" listeners=\"TestOutputStreamer\" />"
            "   </dataStreams>"
            "</output>"
            ;
        CPPUNIT_ASSERT_THROW( _getManager(xml, "output") , QString );
    }
    {
        // Use Case:
        //     configuration contains multiple output streamers of the same type
        //     but with different identifiers and associated with different streams
        // Expect:
        //     correct associations
        QString xml =
            "<output>"
            "   <streamers>"
            "     <TestOutputStreamer name=\"s1\"/>"
            "     <TestOutputStreamer name=\"s2\"/>"
            "   </streamers>"
            "   <dataStreams>"
            "      <stream name=\"a\" listeners=\"s1,s2\" />"
            "   </dataStreams>"
            "</output>"
            ;
        OutputStreamManager* os = _getManager(xml, "output");
        CPPUNIT_ASSERT(os->connected("a").size() == 2 );
        delete os;
    }
    {
        // Use Case:
        //     configuration contains multiple output streamers
        //     and associated with different streams
        // Expect:
        //     correct associations
        QString xml =
            "<output>"
            "   <streamers>"
            "     <TestOutputStreamer name=\"s1\"/>"
            "     <TestOutputStreamer name=\"s2\"/>"
            "     <TestOutputStreamer name=\"s3\"/>"
            "   </streamers>"
            "   <dataStreams>"
            "      <stream name=\"a\" listeners=\"s1\" />"
            "      <stream name=\"b\" listeners=\"s2\" />"
            "      <stream name=\"all\" listeners=\"s3\" />"
            "   </dataStreams>"
            "</output>"
            ;
        OutputStreamManager* os = _getManager(xml, "output");
        CPPUNIT_ASSERT(os->connected("a").size() == 2 );
        CPPUNIT_ASSERT(os->connected("b").size() == 2 );
        delete os;
    }
    {
        // Use Case:
    // Configuration contains an inactive output streamer
        // that is refered to as a listener
        // Expect:
        //      no associations to be made
        QString xml =
            "<output>"
            "   <streamers>"
            "     <TestOutputStreamer active=\"false\"/>"
            "   </streamers>"
            "   <dataStreams>"
            "      <stream name=\"a\" listeners=\"TestOutputStreamer\" />"
            "   </dataStreams>"
            "</output>"
            ;
        OutputStreamManager* os = _getManager(xml, "output");
        CPPUNIT_ASSERT(os->connected("a").size() == 0 );
        delete os;
    }
}

OutputStreamManager* OutputStreamManagerTest::_getManager(const QString& xml, const QString& base)
{
    _address = Config::TreeAddress() << Config::NodeId(base, "");
    if( xml == "" )
        return new OutputStreamManager(0, _address);
    _config->setXML(xml);
    return new OutputStreamManager(_config, _address);
}

} // namespace pelican
