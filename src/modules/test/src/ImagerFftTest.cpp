#include "ImagerFftTest.h"
#include "ImagerFft.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImagerFftTest );
// class ImagerFFTTest
ImagerFftTest::ImagerFftTest()
    : CppUnit::TestFixture()
{
}

ImagerFftTest::~ImagerFftTest()
{
}

void ImagerFftTest::setUp()
{
}

void ImagerFftTest::tearDown()
{
}

void ImagerFftTest::test_createModule()
{
//    Config config;
    QDomElement config;
    ImagerFft imager(config);

}

} // namespace pelican
