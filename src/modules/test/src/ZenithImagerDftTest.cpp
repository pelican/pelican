#include "ZenithImagerDftTest.h"
#include "ZenithImagerDft.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ZenithImagerDftTest );
// class ZenithImagerDftTest 
ZenithImagerDftTest::ZenithImagerDftTest()
    : CppUnit::TestFixture()
{
}

ZenithImagerDftTest::~ZenithImagerDftTest()
{
}

void ZenithImagerDftTest::setUp()
{
}

void ZenithImagerDftTest::tearDown()
{
}

void ZenithImagerDftTest::test_assign()
{
    ImageData image;
    VisibilityData vis;
    AntennaPositions ant;
    QDomElement config;
    ZenithImagerDft imager(config);
}

} // namespace pelican
