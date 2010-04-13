#include "DataManagerTest.h"
#include "DataManager.h"
#include "WritableData.h"
#include "utility/Config.h"

#include "utility/memCheck.h"

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
