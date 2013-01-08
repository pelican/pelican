#include "core/test/DataTypesTest.h"
#include "core/DataTypes.h"
#include "core/test/TestServiceAdapter.h"
#include "core/test/TestStreamAdapter.h"
#include "data/DataSpec.h"

namespace pelican {

using test::TestServiceAdapter;
using test::TestStreamAdapter;

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
    TestServiceAdapter service_adapter;
    TestStreamAdapter stream_adapter;
    {
        // Use case:
        // Single DataRequirement
        // Adapter is of the correct type
        // Expect corresponding dataReq to be the same
        DataSpec req;
        req.addServiceData("service1");
        req.addStreamData("stream1");
        DataTypes dt;
        dt.setAdapter("service1", &service_adapter);
        dt.setAdapter("stream1", &stream_adapter);
        dt.addData(req);
        CPPUNIT_ASSERT_EQUAL(1, dt.dataSpec().size() );
        DataSpec out = dt.dataSpec()[0];
        CPPUNIT_ASSERT( req.streamData().values()[0] == out.streamData().values()[0] );
        CPPUNIT_ASSERT( req.serviceData().values()[0] == out.serviceData().values()[0] );
    }
    {
        // Use case:
        // Single DataRequirement
        // Adapter is of a different type
        // Expect corresponding dataReq to be adjusted to adpater
        DataSpec req;
        req.addStreamData("service1");
        req.addServiceData("stream1");
        DataTypes dt;
        dt.setAdapter("service1", &service_adapter);
        dt.setAdapter("stream1", &stream_adapter);
        dt.addData(req);
        CPPUNIT_ASSERT_EQUAL(1, dt.dataSpec().size() );
        const DataSpec& out = dt.dataSpec()[0];
        CPPUNIT_ASSERT( (req.streamData().values()[0]) == (out.serviceData().values()[0]) );
        CPPUNIT_ASSERT( req.serviceData().values()[0] == out.streamData().values()[0] );
    }
}

} // namespace pelican
