#include "VisibilityDataTest.h"
#include "data/VisibilityData.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( VisibilityDataTest );
// class VisibilityDataTest
VisibilityDataTest::VisibilityDataTest()
    : CppUnit::TestFixture()
{
}

VisibilityDataTest::~VisibilityDataTest()
{
}

void VisibilityDataTest::setUp()
{
}

void VisibilityDataTest::tearDown()
{
}

/**
 * @details
 * Tests the various accessor methods for the visibility data blob
 */
void VisibilityDataTest::test_accessorMethods()
{
    // Use Case
    // Construct a visibility data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    VisibilityData data(nAntennas, nChannels, nPols);

    // Fill the visibility matrix.
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; aj < nAntennas; aj++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    data(ai, aj, c, p) = std::complex<real_t>(index);
                }
            }
        }
    }

    // Read it out again.
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; aj < nAntennas; aj++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    CPPUNIT_ASSERT_EQUAL( std::complex<real_t>(index), data(ai, aj, c, p) );
                }
            }
        }
    }
}

} // namespace pelican
