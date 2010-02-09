#include "ImagerDftTest.h"
#include "ImagerDft.h"
#include "utility/Config.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImagerDftTest );

// class ImagerDftTest
ImagerDftTest::ImagerDftTest()
    : CppUnit::TestFixture()
{
}

ImagerDftTest::~ImagerDftTest()
{
}

void ImagerDftTest::setUp()
{
}

void ImagerDftTest::tearDown()
{
}

void ImagerDftTest::test_createModule()
{
//    Config config;
    QDomElement config;
    ImagerDft imager(config);

}

} // namespace pelican
