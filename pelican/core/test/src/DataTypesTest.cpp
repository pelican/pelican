#include "DataTypesTest.h"
#include "DataTypes.h"
#include "adapters/test/TestAdapter.h"
#include "adapters/test/TestStreamAdapter.h"
#include "data/DataRequirements.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataTypesTest );
/**
 *@details DataTypesTest 
 */
DataTypesTest::DataTypesTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataTypesTest::~DataTypesTest()
{
}

void DataTypesTest::setUp()
{
}

void DataTypesTest::tearDown()
{
}

void DataTypesTest::test_method()
{
    TestAdapter service_adapter;
    TestStreamAdapter stream_adapter;
    {
        // Use case:
        // Single DataRequirement
        // Adapter is of the correct type
        // Expect corresponding dataReq to be the same
        DataRequirements req;
        req.addServiceData("service1");
        req.addStreamData("stream1");
        DataTypes dt;
        dt.addData(req);
        dt.setAdapter("service1", &service_adapter);
        dt.setAdapter("stream1", &stream_adapter);
        CPPUNIT_ASSERT_EQUAL(1, dt.dataRequirements().size() );
        DataRequirements out = dt.dataRequirements()[0];
        CPPUNIT_ASSERT( req.streamData().values()[0] == out.streamData().values()[0] );
        CPPUNIT_ASSERT( req.serviceData().values()[0] == out.serviceData().values()[0] );
    }
    {
        // Use case:
        // Single DataRequirement
        // Adapter is of a different type
        // Expect corresponding dataReq to be adjusted to adpater
        DataRequirements req;
        req.addStreamData("service1");
        req.addServiceData("stream1");
        DataTypes dt;
        dt.addData(req);
        dt.setAdapter("service1", &service_adapter);
        dt.setAdapter("stream1", &stream_adapter);
        CPPUNIT_ASSERT_EQUAL(1, dt.dataRequirements().size() );
        DataRequirements out = dt.dataRequirements()[0];
        CPPUNIT_ASSERT( req.streamData().values()[0] == out.serviceData().values()[0] );
        CPPUNIT_ASSERT( req.serviceData().values()[0] == out.streamData().values()[0] );
    }
}

} // namespace pelican
