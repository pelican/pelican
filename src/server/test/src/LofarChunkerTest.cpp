#include "LofarChunkerTest.h"
#include "LofarChunker.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( LofarChunkerTest );
// class LofarChunkerTest
LofarChunkerTest::LofarChunkerTest()
    : CppUnit::TestFixture()
{
}

LofarChunkerTest::~LofarChunkerTest()
{
}

void LofarChunkerTest::setUp()
{
}

void LofarChunkerTest::tearDown()
{
}

void LofarChunkerTest::test_method()
{
    try {
        std::cout << "---------------------------------" << std::endl;
        std::cout << "Starting LofarChunker test" << std::endl;
        DataManager dataManager;
        dataManager.getStreamBuffer("Test");
        LofarChunker chunker("Test", "127.0.0.1", 8060);
        chunker._nPackets = 10;
        chunker.newDevice();
        chunker.setDataManager(&dataManager);
        chunker.next(NULL);
        std::cout << "Finished LofarChunker test" << std::endl;
    } catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

} // namespace pelican
