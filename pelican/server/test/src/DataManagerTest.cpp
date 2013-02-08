#include "server/test/DataManagerTest.h"
#include "server/DataManager.h"
#include "server/WritableData.h"
#include "utility/Config.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataManagerTest );
// class DataManagerTest
DataManagerTest::DataManagerTest()
    : CppUnit::TestFixture()
{
}

DataManagerTest::~DataManagerTest()
{
}

void DataManagerTest::setUp()
{
}

void DataManagerTest::tearDown()
{
}

void DataManagerTest::test_getWritable()
{
    {
        // Use Case:
        // call get Writable with an unknown type
        // Expect:
        // return an invalid Writable
        Config config;
        DataManager d(&config);
        WritableData data = d.getWritableData("anything",100);
        CPPUNIT_ASSERT(!data.isValid());
    }

}

} // namespace pelican
