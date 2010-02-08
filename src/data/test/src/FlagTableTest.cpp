#include "FlagTableTest.h"
#include "data/FlagTable.h"
//#define TIMER_ENABLE
#include "utility/pelicanTimer.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( FlagTableTest );
// class FlagTableTest
FlagTableTest::FlagTableTest()
    : CppUnit::TestFixture()
{
}

FlagTableTest::~FlagTableTest()
{
}

void FlagTableTest::setUp()
{
}

void FlagTableTest::tearDown()
{
}

/**
 * @details
 * Tests the indexed accessor methods for the flag data blob
 * using a 2D antenna matrix.
 */
void FlagTableTest::test_accessorMethodsIndexed()
{
    // Use Case
    // Construct a flag data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 64;
    const unsigned nPols = 2;
    FlagTable data(nAntennas, nChannels, nPols);

    // Fill the flag matrix for timing purposes.
    TIMER_START
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    unsigned char val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
                    data(ai, aj, c, p) = val;
                }
            }
        }
    }
    TIMER_STOP("Flag table (indexed) data write time")

    // Fill the flag matrix and read it out again.
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    unsigned char val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
                    data(ai, aj, c, p) = val;
                    CPPUNIT_ASSERT_EQUAL( val, data(ai, aj, c, p) );
                    CPPUNIT_ASSERT_EQUAL( val, data(index) );
                }
            }
        }
    }

    // Use Case
    // Test getting the memory address of the first element.
    unsigned char val, *ptr = NULL;
    unsigned index = 0;
    ptr = data.ptr();
    CPPUNIT_ASSERT( FlagTable::FLAG_AUTOCORR == *ptr );

    // Use Case
    // Test getting the memory address of the second polarisation.
    ptr = data.ptr(1);
    index = nAntennas * nAntennas * nChannels;
    val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
    CPPUNIT_ASSERT_EQUAL( val, *ptr );

    // Use Case
    // Test getting the memory address of the middle channel in the first polarisation.
    ptr = data.ptr(nChannels / 2, 0);
    index = nAntennas * nAntennas * nChannels / 2;
    val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
    CPPUNIT_ASSERT_EQUAL( val, *ptr );

    // Use Case
    // Test getting the memory address of the middle channel in the second polarisation.
    ptr = data.ptr(nChannels / 2, 1);
    index = nAntennas * nAntennas * 3 * nChannels / 2;
    val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
    CPPUNIT_ASSERT_EQUAL( val, *ptr );

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
void FlagTableTest::test_accessorMethodsLinear()
{
    // Use Case
    // Construct a flag data set and test each of the accessor methods.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 64;
    const unsigned nPols = 2;
    const unsigned nTotal = nPols * nChannels * nAntennas * nAntennas;
    FlagTable data(nAntennas, nChannels, nPols);

    // Fill the flag matrix for timing purposes.
    TIMER_START
    for (unsigned index = 0; index < nTotal; index++) {
        unsigned char val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
        data[index] = val;
    }
    TIMER_STOP("Flag table (linear) data write time")

    // Fill the flag matrix and read it out again.
    for (unsigned index = 0; index < nTotal; index++) {
        unsigned char val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
        data[index] = val;
        CPPUNIT_ASSERT_EQUAL( val, data(index) );
        CPPUNIT_ASSERT_EQUAL( val, data[index] );
    }

    // Use Case
    // Test getting the memory address of the first element.
    unsigned char* ptr = NULL;
    ptr = data.ptr();
    CPPUNIT_ASSERT( FlagTable::FLAG_AUTOCORR == *ptr );
}

/**
 * @details
 * Tests trying to obtain the memory address at the start of an empty blob.
 */
void FlagTableTest::test_emptyBlob()
{
    // Use Case
    // Test getting the memory address of the first element.
    // Expect NULL to be returned.
    FlagTable data;
    unsigned char* ptr = data.ptr();
    CPPUNIT_ASSERT( ptr  == NULL );
}

/**
 * @details
 * Tests trying to resize empty and pre-sized flag data blobs.
 */
void FlagTableTest::test_resize()
{
    // Use Case
    // Test trying to resize an empty blob.
    FlagTable data;
    data.resize(96, 128, 2);
    unsigned char* ptr = data.ptr(1);
    CPPUNIT_ASSERT( ptr  != NULL );

    // Use Case
    // Try to resize the blob again.
    data.resize(96, 512, 2);
    ptr = data.ptr(255, 1);
    CPPUNIT_ASSERT( ptr  != NULL );
}

} // namespace pelican
