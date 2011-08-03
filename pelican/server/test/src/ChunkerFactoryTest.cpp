#include "ChunkerFactoryTest.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/Config.h"
#include "pelican/server/test/TestUdpChunker.h"

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
    try {
        // Create a test configuration.
        Config config;
        QString serverXml =
                "<chunkers>"
                ""
                "   <TestUdpChunker name=\"1\">"
                "       <connection host=\"127.0.0.1\" port=\"2002\"/>"
                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
                "   </TestUdpChunker>"
                ""
                "   <TestUdpChunker name=\"2\">"
                "       <connection host=\"127.0.0.1\" port=\"2003\"/>"
                "       <data type=\"VisibilityData\" chunkSize=\"512\"/>"
                "   </TestUdpChunker>"
                ""
                "</chunkers>";
        config.setFromString("", serverXml);

        // Create the chunker factory.
        FactoryConfig<AbstractChunker> factory(&config, "server", "chunkers");

        // Create the chunker.
        AbstractChunker* chunker = factory.create("TestUdpChunker", "2");

        // Assert that the chunker has the correct configuration.
        CPPUNIT_ASSERT_EQUAL(1, chunker->chunkTypes().size());
        CPPUNIT_ASSERT(chunker->chunkTypes().at(0) == "VisibilityData");
        CPPUNIT_ASSERT(chunker->host() == "127.0.0.1");
        CPPUNIT_ASSERT(chunker->port() == 2003);
    }

    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
