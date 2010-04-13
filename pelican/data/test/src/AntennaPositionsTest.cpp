#include "pelican/data/test/AntennaPositionsTest.h"
#include "pelican/data/AntennaPositions.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AntennaPositionsTest );
// class DataRequirementsTest
AntennaPositionsTest::AntennaPositionsTest()
    : CppUnit::TestFixture()
{
}

AntennaPositionsTest::~AntennaPositionsTest()
{
}

void AntennaPositionsTest::setUp()
{
}

void AntennaPositionsTest::tearDown()
{
}


/**
 * @details
 * Tests the various accessor methods for the antenna positions data blob.
 */
void AntennaPositionsTest::test_accessorMethods()
{
    unsigned nAnt = 96;
    AntennaPositions antPos(nAnt);
    for (unsigned i = 0; i < nAnt; i++) {
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

    for (unsigned i = 0; i < nAnt; i++) {
        real_t xpos = static_cast<real_t>(i + 0.25);
        real_t ypos = static_cast<real_t>(i + 0.5);
        real_t zpos = static_cast<real_t>(i + 0.75);

        double delta = 1.0e-5;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(xpos, antPos.x(i), delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(xpos, xPtr[i], delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(xpos, x[i], delta);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(ypos, antPos.y(i), delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ypos, yPtr[i], delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(ypos, y[i], delta);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(zpos, antPos.z(i), delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(zpos, zPtr[i], delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(zpos, z[i], delta);
    }
}


} // namespace pelican
