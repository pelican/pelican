#include "AbstractPipelineTest.h"
#include "AbstractPipeline.h"
#include "pelican/data/test/TestDataBlob.h"
#include "TestPipeline.h"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AbstractPipelineTest );
/**
 *@details AbstractPipelineTest 
 */
AbstractPipelineTest::AbstractPipelineTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
AbstractPipelineTest::~AbstractPipelineTest()
{
}

void AbstractPipelineTest::setUp()
{
}

void AbstractPipelineTest::tearDown()
{
}

void AbstractPipelineTest::test_createBlobs()
{
        unsigned int number = 10;
        test::TestPipeline p;
        QList<test::TestDataBlob*> buffer = p.createBlobs<test::TestDataBlob>("TestDataBlob", number);
        CPPUNIT_ASSERT_EQUAL( number, (unsigned int)buffer.size() ); // check correct number
        CPPUNIT_ASSERT( buffer[0] != buffer[1] ); // check different blobs
        CPPUNIT_ASSERT( buffer[2]->type() == "TestDataBlob" ); // check datablob type
}

} // namespace pelican
