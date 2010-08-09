#include "FileChunkerTest.h"
#include "FileChunker.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( FileChunkerTest );
/**
 * @details Constructs a FileChunkerTest object.
 */
FileChunkerTest::FileChunkerTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the FileChunkerTest object.
 */
FileChunkerTest::~FileChunkerTest()
{
}

void FileChunkerTest::setUp()
{
}

void FileChunkerTest::tearDown()
{
}

void FileChunkerTest::test_startup()
{
    // Use case:
    // Start the Chunker with an exiting file as its target
    // Expect:
    // A memory request to be made, and filled with the initial content
    FileChunker fc;
    fc.setFile(_testFile);
    ChunkerTester tester(fc);
    tester->run();
    CPPUNIT_ASSERT_EQUAL( 1, tester->writeRequestCount() );
    CPPUNIT_ASSERT_EQUAL( tester->getData().size(), _testData.size() );
}

void FileChunkerTest::test_update()
{
    // Use case:
    //   Update the file with some data
    // Expect:
    //   A memory request to be made, and filled with the new content
    FileChunker fc;
    fc.setFile(_testFile);
    ChunkerTester tester(fc);
    CPPUNIT_ASSERT( tester->runOnce() );
    CPPUNIT_ASSERT_EQUAL( 1, tester->writeRequestCount() );
    QString moredata("moredata");
    _updateFile(moredata);
    CPPUNIT_ASSERT( tester->runOnce() );
    CPPUNIT_ASSERT_EQUAL( 2, tester->writeRequestCount() );
    CPPUNIT_ASSERT_EQUAL( tester->getData().size(), _testData.size()  + moredata.size());
}

} // namespace pelican
