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
 * Tests the indexed accessor methods for the visibility data blob
 * using a 2D antenna matrix.
 */
void VisibilityDataTest::test_accessorMethodsIndexed()
{
    // Use Case
    // Construct a visibility data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 64;
    const unsigned nPols = 2;
    VisibilityData data(nAntennas, nChannels, nPols);

    // Fill the visibility matrix and read it out again.
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    std::complex<real_t> val(index);
                    data(ai, aj, c, p) = val;
                    CPPUNIT_ASSERT_EQUAL( val, data(ai, aj, c, p) );
                    CPPUNIT_ASSERT_EQUAL( val, data(index) );
                }
            }
        }
    }

    // Use Case
    // Test getting the memory address of the first element.
    std::complex<real_t>* ptr = NULL;
    ptr = data.ptr();
    CPPUNIT_ASSERT_EQUAL( std::complex<real_t>(0), *ptr );

    // Use Case
    // Test getting the memory address of the second polarisation.
    ptr = data.ptr(1);
    CPPUNIT_ASSERT_EQUAL( std::complex<real_t>(nAntennas * nAntennas * nChannels), *ptr );

    // Use Case
    // Test getting the memory address of the middle channel in the first polarisation.
    ptr = data.ptr(nChannels / 2, 0);
    CPPUNIT_ASSERT_EQUAL( std::complex<real_t>(nAntennas * nAntennas * nChannels / 2), *ptr );

    // Use Case
    // Test getting the memory address of the middle channel in the second polarisation.
    ptr = data.ptr(nChannels / 2, 1);
    CPPUNIT_ASSERT_EQUAL( std::complex<real_t>(nAntennas * nAntennas * 3 * nChannels / 2), *ptr );

    // Use Case
    // Test getting the memory address of an element out of range.
    // Expect NULL to be returned.
    ptr = data.ptr(nChannels / 2, nPols);
    CPPUNIT_ASSERT( ptr  == NULL );
}

/**
 * @details
 * Tests the linear accessor methods for the visibility data blob
 * using a 2D antenna matrix.
 */
void VisibilityDataTest::test_accessorMethodsLinear()
{
    // Use Case
    // Construct a visibility data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 64;
    const unsigned nPols = 2;
    const unsigned nTotal = nPols * nChannels * nAntennas * nAntennas;
    VisibilityData data(nAntennas, nChannels, nPols);

    // Fill the visibility matrix and read it out again.
    for (unsigned index = 0; index < nTotal; index++) {
        std::complex<real_t> val(index);
        data[index] = val;
        CPPUNIT_ASSERT_EQUAL( val, data(index) );
        CPPUNIT_ASSERT_EQUAL( val, data[index] );
    }

    // Use Case
    // Test getting the memory address of the first element.
    std::complex<real_t>* ptr = NULL;
    ptr = data.ptr();
    CPPUNIT_ASSERT_EQUAL( std::complex<real_t>(0), *ptr );
}

/**
 * @details
 * Tests trying to obtain the memory address at the start of an empty blob.
 */
void VisibilityDataTest::test_emptyBlob()
{
    // Use Case
    // Test getting the memory address of the first element.
    // Expect NULL to be returned.
    VisibilityData data;
    std::complex<real_t>* ptr = data.ptr();
    CPPUNIT_ASSERT( ptr  == NULL );
}

/**
 * @details
 * Tests trying to resize empty and pre-sized visibility data blobs.
 */
void VisibilityDataTest::test_resize()
{
    // Use Case
    // Test trying to resize an empty blob.
    VisibilityData data;
    data.resize(96, 128, 2);
    std::complex<real_t>* ptr = data.ptr(1);
    CPPUNIT_ASSERT( ptr  != NULL );

    // Use Case
    // Try to resize the blob again.
    data.resize(96, 512, 2);
    ptr = data.ptr(255, 1);
    CPPUNIT_ASSERT( ptr  != NULL );
}

/**
 * @details
 * Test swap of antenna data with itself.
 */
void VisibilityDataTest::test_swap_same()
{
    // Use Case
    // Test trying to swap antenna data with itself.
    const unsigned nAntennas = 3;
    const unsigned nChannels = 2;
    const unsigned nPols = 2;
    const unsigned nTotal = nPols * nChannels * nAntennas * nAntennas;
    VisibilityData data(nAntennas, nChannels, nPols);

    // Fill the visibility matrix.
    for (unsigned index = 0; index < nTotal; index++) {
        std::complex<real_t> val(index);
        data[index] = val;
    }

    // Swap antenna 1 with itself.
    data.swapAntennaData(1, 1, 0, 0);

    // Data should be the same as it was before.
    for (unsigned index = 0; index < nTotal; index++) {
        std::complex<real_t> val(index);
        CPPUNIT_ASSERT_EQUAL( val, data(index) );
    }
}

/**
 * @details
 * Tests trying to swap antenna data rows and columns.
 */
void VisibilityDataTest::test_swap_twice()
{
    // Use Case
    // Test trying to swap antenna data.
    const unsigned nAntennas = 3;
    const unsigned nChannels = 2;
    const unsigned nPols = 2;
    const unsigned nTotal = nPols * nChannels * nAntennas * nAntennas;
    VisibilityData data(nAntennas, nChannels, nPols);

    // Fill the visibility matrix.
    for (unsigned index = 0; index < nTotal; index++) {
        std::complex<real_t> val(index);
        data[index] = val;
    }

    // Put antenna index 1 at the back of the matrix.
    data.swapAntennaData(1, 2, 0, 0);

    // Put antenna 2 (now at position 1) at the back of the matrix.
    data.swapAntennaData(1, 2, 0, 0);

    // Data should now be the same as it was before.
    for (unsigned index = 0; index < nTotal; index++) {
        std::complex<real_t> val(index);
        CPPUNIT_ASSERT_EQUAL( val, data(index) );
    }

    // Test channel 1, polarisation 1 in the same way.

    // Put antenna index 1 at the back of the matrix.
    data.swapAntennaData(1, 2, 1, 1);

    // Put antenna 2 (now at position 1) at the back of the matrix.
    data.swapAntennaData(1, 2, 1, 1);

    // Data should now be the same as it was before.
    for (unsigned index = 0; index < nTotal; index++) {
        std::complex<real_t> val(index);
        CPPUNIT_ASSERT_EQUAL( val, data(index) );
    }
}

} // namespace pelican
