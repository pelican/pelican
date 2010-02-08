#include "AntennaGainsTest.h"
#include "data/AntennaGains.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AntennaGainsTest );
// class AntennaGainsTest
AntennaGainsTest::AntennaGainsTest()
    : CppUnit::TestFixture()
{
}

AntennaGainsTest::~AntennaGainsTest()
{
}

void AntennaGainsTest::setUp()
{
}

void AntennaGainsTest::tearDown()
{
}

/**
 * @details
 * Tests the indexed accessor methods for the antenna gains blob
 * using a 2D antenna matrix.
 */
void AntennaGainsTest::test_accessorMethodsIndexed()
{
    // Use Case
    // Construct a gains data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 64;
    const unsigned nPols = 2;
    AntennaGains data(nAntennas, nChannels, nPols);

    // Fill the gain matrix and read it out again.
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
 * Tests the linear accessor methods for the antenna gains data blob
 * using a 2D antenna matrix.
 */
void AntennaGainsTest::test_accessorMethodsLinear()
{
    // Use Case
    // Construct a gain data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 64;
    const unsigned nPols = 2;
    const unsigned nTotal = nPols * nChannels * nAntennas * nAntennas;
    AntennaGains data(nAntennas, nChannels, nPols);

    // Fill the gain matrix and read it out again.
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
void AntennaGainsTest::test_emptyBlob()
{
    // Use Case
    // Test getting the memory address of the first element.
    // Expect NULL to be returned.
    AntennaGains data;
    std::complex<real_t>* ptr = data.ptr();
    CPPUNIT_ASSERT( ptr  == NULL );
}

/**
 * @details
 * Tests trying to resize empty and pre-sized gain data blobs.
 */
void AntennaGainsTest::test_resize()
{
    // Use Case
    // Test trying to resize an empty blob.
    AntennaGains data;
    data.resize(96, 128, 2);
    std::complex<real_t>* ptr = data.ptr(1);
    CPPUNIT_ASSERT( ptr  != NULL );

    // Use Case
    // Try to resize the blob again.
    data.resize(96, 512, 2);
    ptr = data.ptr(255, 1);
    CPPUNIT_ASSERT( ptr  != NULL );
}

} // namespace pelican
