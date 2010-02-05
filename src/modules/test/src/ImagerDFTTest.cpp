#include "ImagerDFTTest.h"
#include "ImagerDFT.h"
#include "utility/Config.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImagerDFTTest );

// class ImagerDFTTest
ImagerDFTTest::ImagerDFTTest()
    : CppUnit::TestFixture()
{
}

ImagerDFTTest::~ImagerDFTTest()
{
}

void ImagerDFTTest::setUp()
{
}

void ImagerDFTTest::tearDown()
{
}

void ImagerDFTTest::test_createModule()
{
//    Config config;
    QDomElement config;
    ImagerDFT imager(config);

}

} // namespace pelican
