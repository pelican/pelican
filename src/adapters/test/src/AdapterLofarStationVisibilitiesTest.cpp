#include "AdapterLofarStationVisibilitiesTest.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AdapterLofarStationVisibilitiesTest );
// class FileAdapterLofarVisibilitiesTest
AdapterLofarStationVisibilitiesTest::AdapterLofarStationVisibilitiesTest()
    : CppUnit::TestFixture()
{
}

AdapterLofarStationVisibilitiesTest::~AdapterLofarStationVisibilitiesTest()
{
}

void AdapterLofarStationVisibilitiesTest::setUp()
{
}

void AdapterLofarStationVisibilitiesTest::tearDown()
{
}

void AdapterLofarStationVisibilitiesTest::test_method()
{
    QDomElement e;
    ConfigNode config(e);
    AdapterLofarStationVisibilities adapter(config);


}

} // namespace pelican
