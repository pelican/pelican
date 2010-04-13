#include "ChunkerFactoryTest.h"
#include "ChunkerFactory.h"
#include "TestUdpChunker.h"
#include "pelican/utility/Config.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ChunkerFactoryTest );

/**
 * @details ChunkerFactoryTest constructor.
 */
ChunkerFactoryTest::ChunkerFactoryTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * Destroys the ChunkerFactoryTest object.
 */
ChunkerFactoryTest::~ChunkerFactoryTest()
{
}

/**
 * @details
 * Initialisation routine run before each test method.
 */
void ChunkerFactoryTest::setUp()
{
}

/**
 * @details
 * Clean-up routine run after each test method.
 */
void ChunkerFactoryTest::tearDown()
{
}

/**
 * @details
 * Tests creating a new chunker using the factory create() method.
 */
void ChunkerFactoryTest::test_create()
{
    //    try {
    //        // Create a test configuration.
    //        Config config;
    //        QString serverXml =
    //                "<chunkers>"
    //                "   <TestUdpChunker name=\"1\">"
    //                "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
    //                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
    //                "   </TestUdpChunker>"
    //                "   <TestUdpChunker name=\"2\">"
    //                "       <connection host=\"127.0.0.1\" port=\"2003\"/>"
    //                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
    //                "   </TestUdpChunker>"
    //                "</chunkers>";
    //        config.setFromString("", serverXml);
    //
    //        // Create the chunker factory.
    //        Config::TreeAddress_t address;
    //        address.append(Config::NodeId_t("server", ""));
    //        address.append(Config::NodeId_t("chunkers", ""));
    //        ChunkerFactory factory(&config, address);
    //
    //        // Create a chunker with the right configuration.
    //        AbstractChunker* chunker = factory.create<TestUdpChunker>("TestUdpChunker", "2");
    //
    //        // Assert that the chunker has the correct configuration.
    //        CPPUNIT_ASSERT(chunker->type() == "VisibilityData");
    //        CPPUNIT_ASSERT(chunker->host() == "127.0.0.1");
    //        CPPUNIT_ASSERT(chunker->port() == 2003);
    //    }
    //
    //    catch (QString e) {
    //        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    //    }

    try {
        // Create a test configuration.
        Config config;
        QString serverXml =
                "<chunkers>"
                "   <TestUdpChunker name=\"1\">"
                "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
                "   </TestUdpChunker>"
                "   <TestUdpChunker name=\"2\">"
                "       <connection host=\"127.0.0.1\" port=\"2003\"/>"
                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
                "   </TestUdpChunker>"
                "</chunkers>";
        config.setFromString("", serverXml);

        // Create the chunker factory.
        Config::TreeAddress address;
        address.append(Config::NodeId("server", ""));
        address.append(Config::NodeId("chunkers", ""));
        ChunkerFactory factory(&config, address);


        //typedef ChunkerClassGenerator<TestUdpChunker> chunkerGenerator;
        //factory.registerChunkerType(boost::shared_ptr<chunkerGenerator>(new chunkerGenerator("TestUdpChunker")));
        PELICAN_DECLARE_CHUNKER(TestUdpChunker);

        AbstractChunker* chunker = factory.create("TestUdpChunker", "2");


        // Assert that the chunker has the correct configuration.
        CPPUNIT_ASSERT(chunker->type() == "VisibilityData");
        CPPUNIT_ASSERT(chunker->host() == "127.0.0.1");
        CPPUNIT_ASSERT(chunker->port() == 2003);
    }

    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
