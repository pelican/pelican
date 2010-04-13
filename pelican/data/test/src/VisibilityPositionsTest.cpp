#include "pelican/data/test/VisibilityPositionsTest.h"
#include "pelican/data/VisibilityPositions.h"
#include "pelican/utility/memCheck.h"

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
    unsigned nAnt = 96;
    double c = 299792458.0;
    double refFreq = 1.0e8;
    double freqInc = 1.0e6;
    VisibilityPositions visPos(nAnt);
    visPos.refChannel() = 0;
    visPos.refFreq() = refFreq;
    visPos.freqInc() = freqInc;

    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++) {
            visPos.u(i, j) = static_cast<real_t>((j - i) * (refFreq / c));
            visPos.v(i, j) = static_cast<real_t>((2 * j - i) * (refFreq / c));
            visPos.w(i, j) = static_cast<real_t>(j * (refFreq / c));
        }
    }

    real_t* u = visPos.uPtr();
    real_t* v = visPos.vPtr();
    real_t* w = visPos.wPtr();
    double delta = 1.0e-5;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.01, visPos.freqScaleFactor(1), delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, visPos.freqScaleFactor(20), delta);

    for (unsigned j = 0; j < nAnt; j++) {
        for (unsigned i = 0; i < nAnt; i++) {
            real_t uval = static_cast<real_t>((j - i) * (refFreq / c));
            real_t vval = static_cast<real_t>((2 * j - i) * (refFreq / c));
            real_t wval = static_cast<real_t>(j * (refFreq / c));
            CPPUNIT_ASSERT_DOUBLES_EQUAL(uval, visPos.u(i, j), delta);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(uval, u[j * nAnt + i], delta);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(vval, visPos.v(i, j), delta);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(vval, v[j * nAnt + i], delta);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(wval, visPos.w(i, j), delta);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(wval, w[j * nAnt + i], delta);
        }
    }
}


} // namespace pelican
