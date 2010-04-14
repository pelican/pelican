#include "FactoryTest.h"
#include "pelican/utility/Factory.h"
#include "pelican/utility/Config.h"
#include "pelican/server/test/TestUdpChunker.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( FactoryTest );

/**
 * @details FactoryTest constructor.
 */
FactoryTest::FactoryTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * Destroys the FactoryTest object.
 */
FactoryTest::~FactoryTest()
{
}

/**
 * @details
 * Initialisation routine run before each test method.
 */
void FactoryTest::setUp()
{
}

/**
 * @details
 * Clean-up routine run after each test method.
 */
void FactoryTest::tearDown()
{
}

/**
 * @details
 * Tests creating a new chunker using the factory create() method.
 */
void FactoryTest::test_create()
{
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
        Factory<AbstractChunker> factory(&config, address);

        // Create the chunker.
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
