#include "VisibilityPositionsTest.h"
#include "VisibilityPositions.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( VisibilityPositionsTest );
// class DataRequirementsTest 
VisibilityPositionsTest::VisibilityPositionsTest()
    : CppUnit::TestFixture()
{
}

VisibilityPositionsTest::~VisibilityPositionsTest()
{
}

void VisibilityPositionsTest::setUp()
{
}

void VisibilityPositionsTest::tearDown()
{
}


/**
 * @details
 * Tests the various accessor methods for the visibility positions data blob
 */
void VisibilityPositionsTest::test_accessorMethods()
{
    int nAnt = 96;
    double c = 299792458.0;
    double refFreq = 1.0e8;
    double freqInc = 1.0e6;
    VisibilityPositions visPos(nAnt);
    visPos.refChannel() = 0;
    visPos.refFreq() = refFreq;
    visPos.freqInc() = freqInc;

    for (int j = 0; j < nAnt; j++) {
        for (int i = 0; i < nAnt; i++) {
            visPos.u(i, j) = static_cast<real_t>((j - i) * (refFreq / c));
            visPos.v(i, j) = static_cast<real_t>((2 * j - i) * (refFreq / c));
            visPos.w(i, j) = static_cast<real_t>(j * (refFreq / c));
        }
    }

    real_t* u = visPos.uPtr();
    real_t* v = visPos.uPtr();
    real_t* w = visPos.uPtr();
    CPPUNIT_ASSERT(visPos.freqScaleFactor(1) == 1.01);
    CPPUNIT_ASSERT(visPos.freqScaleFactor(20) == 1.2);

    for (int j = 0; j < nAnt; j++) {
        for (int i = 0; i < nAnt; i++) {
            real_t uval = static_cast<real_t>((j - i) * (refFreq / c));
            real_t vval = static_cast<real_t>((2 * j - i) * (refFreq / c));
            real_t wval = static_cast<real_t>(j * (refFreq / c));
            CPPUNIT_ASSERT(visPos.u(i,j) == uval);
            CPPUNIT_ASSERT(u[j * nAnt + i] == uval);
            CPPUNIT_ASSERT(visPos.v(i,j) == vval);
            CPPUNIT_ASSERT(v[j * nAnt + i] == uval);
            CPPUNIT_ASSERT(visPos.w(i,j) == wval);
            CPPUNIT_ASSERT(w[j * nAnt + i] == uval);
        }
    }
}


} // namespace pelican
