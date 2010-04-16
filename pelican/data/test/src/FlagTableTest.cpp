#include "pelican/data/test/FlagTableTest.h"
#include "pelican/data/FlagTable.h"
#include "pelican/utility/constants.h"
#include "pelican/utility/pelicanTimer.h"
#include "pelican/utility/memCheck.h"

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
    const unsigned nChannels = 4;
    const unsigned nPols = 2;
    std::vector<unsigned> channels(nChannels);
    FlagTable data(nAntennas, channels, POL_BOTH);

    // Fill the flag matrix for timing purposes.
    TIMER_START
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            unsigned char* ptr = data.ptr(c, p);
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    unsigned char val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
                    ptr[aj * nAntennas + ai] = val;
                }
            }
        }
    }
    TIMER_STOP("FlagTable::operator(). Indexed data write time")

    // Fill the flag matrix and read it out again.
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            unsigned char* ptr = data.ptr(c, p);
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    unsigned index = ai + nAntennas * (aj + nAntennas * (c + nChannels * p));
                    unsigned char val = (index % 2 == 0) ? FlagTable::FLAG_AUTOCORR : 0;
                    ptr[aj * nAntennas + ai] = val;
                    CPPUNIT_ASSERT_EQUAL( val, ptr[aj * nAntennas + ai]);
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
    const unsigned nChannels = 4;
    const unsigned nPols = 2;
    const unsigned nTotal = nPols * nChannels * nAntennas * nAntennas;
    std::vector<unsigned> channels(nChannels);
    FlagTable data(nAntennas, channels, POL_BOTH);

    // Fill the flag matrix for timing purposes.
    TIMER_START
    for (unsigned index = 0; index < nTotal; index++) {
        unsigned char val = (index % 2 == 0) ?
                FlagTable::FLAG_AUTOCORR : 0;
        data[index] = val;
    }
    TIMER_STOP("FlagTable::operator[]. Linear data write time")

    // Fill the flag matrix and read it out again.
    for (unsigned index = 0; index < nTotal; index++) {
        unsigned char val = (index % 2 == 0) ?
                FlagTable::FLAG_AUTOCORR : 0;
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
 * Tests trying to write flag table entries.
 */
void FlagTableTest::test_flag()
{
    // Initialise test.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 4;
    const unsigned nPols = 2;

    {
        // Use Case
        // Test flagging a single antenna pair.
        std::vector<unsigned> channels(nChannels);
        FlagTable data(nAntennas, channels, POL_BOTH);
        const unsigned sAntennai = 45;
        const unsigned sAntennaj = 21;
        const unsigned sChannel = 1;
        const unsigned sPol = 1;
        TIMER_START
        data.flag(sAntennai, sAntennaj, sChannel, sPol, FlagTable::FLAG_UVDIST);
        TIMER_STOP("FlagTable::flag(). One antenna pair in one channel")
        for (unsigned p = 0; p < nPols; ++p) {
            for (unsigned c = 0; c < nChannels; ++c) {
                for (unsigned aj = 0; aj < nAntennas; ++aj) {
                    for (unsigned ai = 0; ai < nAntennas; ++ai) {
                        if (ai == sAntennai && aj == sAntennaj && c == sChannel && p == sPol)
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == FlagTable::FLAG_UVDIST);
                        else
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == 0 );
                    }
                }
            }
        }
    }

    {
        // Use Case
        // Test flagging all data from an antenna.
        std::vector<unsigned> channels(nChannels);
        FlagTable data(nAntennas, channels, POL_BOTH);
        const unsigned sAntenna = 67;
        const unsigned sChannel = 2;
        const unsigned sPol = 0;
        TIMER_START
        data.flag(sAntenna, sChannel, sPol, FlagTable::FLAG_AUTOCORR);
        TIMER_STOP("FlagTable::flag(). One antenna")
        for (unsigned p = 0; p < nPols; ++p) {
            for (unsigned c = 0; c < nChannels; ++c) {
                for (unsigned aj = 0; aj < nAntennas; ++aj) {
                    for (unsigned ai = 0; ai < nAntennas; ++ai) {
                        if (c == sChannel && p == sPol) {
                            if (ai == sAntenna || aj == sAntenna)
                                CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == FlagTable::FLAG_AUTOCORR);
                            else
                                CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == 0);
                        }
                        else
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == 0);
                    }
                }
            }
        }
    }

    {
        // Use Case
        // Test flagging all data from one channel.
        std::vector<unsigned> channels(nChannels);
        FlagTable data(nAntennas, channels, POL_BOTH);
        const unsigned sChannel = 3;
        const unsigned sPol = 1;
        TIMER_START
        data.flag(sChannel, sPol, FlagTable::FLAG_RFI_BAD);
        TIMER_STOP("FlagTable::flag(). One channel, all %u antennas", nAntennas)
        for (unsigned p = 0; p < nPols; ++p) {
            for (unsigned c = 0; c < nChannels; ++c) {
                for (unsigned aj = 0; aj < nAntennas; ++aj) {
                    for (unsigned ai = 0; ai < nAntennas; ++ai) {
                        if (c == sChannel && p == sPol)
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == FlagTable::FLAG_RFI_BAD);
                        else
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == 0 );
                    }
                }
            }
        }
    }

    {
        // Use Case
        // Test flagging all data from all channels.
        std::vector<unsigned> channels(nChannels);
        FlagTable data(nAntennas, channels, POL_BOTH);
        const unsigned sChannelStart = 0;
        const unsigned sChannelEnd = nChannels-1;
        const unsigned sPol = 1;
        TIMER_START
        for (unsigned c = sChannelStart; c <= sChannelEnd; c++) {
            data.flag(c, sPol, FlagTable::FLAG_RFI_BAD);
        }
        TIMER_STOP("FlagTable::flag(). One pol, all %u channels, "
                "all %u antennas", nChannels, nAntennas)
        for (unsigned p = 0; p < nPols; ++p) {
            for (unsigned c = 0; c < nChannels; ++c) {
                for (unsigned aj = 0; aj < nAntennas; ++aj) {
                    for (unsigned ai = 0; ai < nAntennas; ++ai) {
                        if (p == sPol)
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == FlagTable::FLAG_RFI_BAD);
                        else
                            CPPUNIT_ASSERT(data.flags(ai, aj, c, p) == 0 );
                    }
                }
            }
        }
    }
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
    std::vector<unsigned> channels(128);
    data.resize(96, channels, POL_BOTH);
    unsigned char* ptr = data.ptr(1);
    CPPUNIT_ASSERT( ptr  != NULL );

    // Use Case
    // Try to resize the blob again.
    channels.resize(512);
    data.resize(96, channels, POL_BOTH);
    ptr = data.ptr(255, 1);
    CPPUNIT_ASSERT( ptr  != NULL );
}

} // namespace pelican
