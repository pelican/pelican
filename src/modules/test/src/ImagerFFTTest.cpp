#include "ImagerFFTTest.h"
#include "ImagerFFT.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImagerFFTTest );
// class ImagerFFTTest
ImagerFFTTest::ImagerFFTTest()
    : CppUnit::TestFixture()
{
}

ImagerFFTTest::~ImagerFFTTest()
{
}

void ImagerFFTTest::setUp()
{
}

void ImagerFFTTest::tearDown()
{
}

void ImagerFFTTest::test_createModule()
{
//    Config config;
    QDomElement config;
    ImagerFFT imager(config);

}

} // namespace pelican
