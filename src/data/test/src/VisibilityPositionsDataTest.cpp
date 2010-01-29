#include "VisibilityPositionsDataTest.h"
#include "VisibilityPositionsData.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( VisibilityPositionsDataTest );
// class DataRequirementsTest 
VisibilityPositionsDataTest::VisibilityPositionsDataTest()
    : CppUnit::TestFixture()
{
}

VisibilityPositionsDataTest::~VisibilityPositionsDataTest()
{
}

void VisibilityPositionsDataTest::setUp()
{
}

void VisibilityPositionsDataTest::tearDown()
{
}


/**
 * @details
 * Tests the various accessor methods for the visibility positions data blob
 */
void VisibilityPositionsDataTest::test_accessorMethods()
{
    int nAnt = 96;
    VisibilityPositions visPos(nAnt);
    visPos.refChannel() = 0;
    visPos.refFreq() = 1.0e8;
    visPos.freqInc() = 1.0e6;

    for (int j = 0; j < nAnt; j++) {
        for (int i = 0; i < nAnt; i++) {
            visPos.u(i, j) = static_cast<real_t>(j - i);
        }
    }

}


} // namespace pelican
