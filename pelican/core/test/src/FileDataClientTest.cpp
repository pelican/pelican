#include "FileDataClientTest.h"
#include "FileDataClient.h"

#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/core/DataTypes.h"
#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( FileDataClientTest );
/**
 * @details Constructs a FileDataClientTest object.
 */
FileDataClientTest::FileDataClientTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the FileDataClientTest object.
 */
FileDataClientTest::~FileDataClientTest()
{
}

void FileDataClientTest::setUp()
{
}

void FileDataClientTest::tearDown()
{
}

void FileDataClientTest::test_method()
{
     { // Use Case:
       // Specifiy a single file and associate it with a Stream
       // Expect:
       // Data in file to be streamed
       Config config;
       ConfigNode configNode;
       DataTypes types;
       FileDataClient client(configNode, types, &config);
       DataRequirements req;
       //client.getData(req);
     }

}

} // namespace pelican
