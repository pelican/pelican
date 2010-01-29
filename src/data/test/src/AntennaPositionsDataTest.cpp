#include "AntennaPositionsDataTest.h"
#include "AntennaPositionsData.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AntennaPositionsDataTest );
// class DataRequirementsTest 
AntennaPositionsDataTest::AntennaPositionsDataTest()
    : CppUnit::TestFixture()
{
}

AntennaPositionsDataTest::~AntennaPositionsDataTest()
{
}

void AntennaPositionsDataTest::setUp()
{
}

void AntennaPositionsDataTest::tearDown()
{
}


/**
 * @details
 * Tests the various accessor methods for the antenna positions data blob
 */
void AntennaPositionsDataTest::test_accessorMethods()
{
    int nAnt = 96;
    AntennaPositions antPos(nAnt);
    for (int i = 0; i < nAnt; i++) {
        antPos.x(i) = static_cast<real_t>(i + 0.25);
        antPos.y(i) = static_cast<real_t>(i + 0.5);
        antPos.z(i) = static_cast<real_t>(i + 0.75);
    }

    CPPUNIT_ASSERT(antPos.nAntennas() == nAnt);
    real_t* xPtr = antPos.xPtr();
    real_t* yPtr = antPos.yPtr();
    real_t* zPtr = antPos.zPtr();
    std::vector<real_t> x = antPos.x();
    std::vector<real_t> y = antPos.y();
    std::vector<real_t> z = antPos.z();

    for (int i = 0; i < nAnt; i++) {
        real_t xpos = static_cast<real_t>(i + 0.25);
        real_t ypos = static_cast<real_t>(i + 0.5);
        real_t zpos = static_cast<real_t>(i + 0.75);

        CPPUNIT_ASSERT(antPos.x(i) == xpos);
        CPPUNIT_ASSERT(xPtr[i] == xpos);
        CPPUNIT_ASSERT(x[i] == xpos);

        CPPUNIT_ASSERT(antPos.y(i) == ypos);
        CPPUNIT_ASSERT(yPtr[i] == ypos);
        CPPUNIT_ASSERT(y[i] == ypos);

        CPPUNIT_ASSERT(antPos.z(i) == zpos);
        CPPUNIT_ASSERT(zPtr[i] == zpos);
        CPPUNIT_ASSERT(z[i] == zpos);
    }

}


} // namespace pelican
