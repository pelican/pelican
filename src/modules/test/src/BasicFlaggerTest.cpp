#include "BasicFlaggerTest.h"
#include "modules/BasicFlagger.h"
#include "data/VisibilityData.h"
#include "data/FlagTable.h"
#include "utility/pelicanTimer.h"
#include <algorithm>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( BasicFlaggerTest );
// class BasicFlaggerTest 
BasicFlaggerTest::BasicFlaggerTest()
    : CppUnit::TestFixture()
{
}

BasicFlaggerTest::~BasicFlaggerTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void BasicFlaggerTest::setUp()
{
    /* Create a new module with empty configuration before each test */
    _basicFlagger = new BasicFlagger(QDomElement());
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void BasicFlaggerTest::tearDown()
{
    /* Delete the module after each test */
    delete _basicFlagger;
}

/**
 * @details
 * Tests the run() method of the BasicFlagger using an empty data hash,
 * and a hash containing no visibilities.
 */
void BasicFlaggerTest::test_run_noData()
{
    // Use Case
    // Pass an empty data blob hash.
    // Expect an exception.
    {
        QHash<QString, DataBlob*> emptyData;
        CPPUNIT_ASSERT_THROW(_basicFlagger->run(emptyData), QString);
    }

    // Use Case
    // Pass a data blob hash containing no visibility data.
    // Expect an exception.
    {
        QHash<QString, DataBlob*> data;
        DataBlob blob;
        data.insert("EmptyBlob", &blob);
        CPPUNIT_ASSERT_THROW(_basicFlagger->run(data), QString);
    }

    // Use Case
    // Pass a data blob hash containing empty visibility data.
    // Expect an exception.
    {
        QHash<QString, DataBlob*> data;
        VisibilityData blob;
        data.insert("VisibilityData", &blob);
        CPPUNIT_ASSERT_THROW(_basicFlagger->run(data), QString);
    }
}

/**
 * @details
 * Tests the run() method of the BasicFlagger using
 * visibility data and flag table.
 */
void BasicFlaggerTest::test_run_withData()
{
    // Use Case
    // Create a set of test visibility data and pass it to the module.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    VisibilityData visData(nAntennas, nChannels, nPols);
    FlagTable flagTable(nAntennas, nChannels, nPols);

    QHash<QString, DataBlob*> data;
    data.insert("VisibilityData", &visData);
    data.insert("FlagTable", &flagTable);

    CPPUNIT_ASSERT_NO_THROW(_basicFlagger->run(data));
}

/**
 * @details
 * Tests the routine to flag the autocorrelations (diagonal terms)
 * in the visibility matrix.
 */
void BasicFlaggerTest::test__flagAutocorrelations()
{
    // Use Case
    // Create a set of test visibility data and get the autocorrelations.
    const unsigned nAntennas = 97;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    const real_t minFraction = 0.25;
    const real_t maxFraction = 1.25;
    VisibilityData visData(nAntennas, nChannels, nPols);
    FlagTable flagTable(nAntennas, nChannels, nPols);
    std::vector<complex_t> medians(nChannels * nPols);

    // Fill the visibility matrix
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    std::complex<real_t> val( sqrt((ai+1)*(aj+1)) );
                    visData(ai, aj, c, p) = val;
                }
            }
            medians[c + p * nChannels] = visData(nAntennas/2, nAntennas/2, c, p);
        }
    }

    // Get the autocorrelations.
    TIMER_START
    _basicFlagger->_flagAutocorrelations(&visData, &medians[0],
            minFraction, maxFraction, &flagTable);
    TIMER_STOP("BasicFlagger::_flagAutocorrelations (%d/%d ant in %d chan, %d pol)",
            flagTable.nFlaggedAntennas(0, 0), nAntennas, nChannels, nPols)
}

/**
 * @details
 * Tests the routine to get the autocorrelations (diagonal terms)
 * from the visibility matrix.
 */
void BasicFlaggerTest::test__getAutocorrelations()
{
    // Use Case
    // Create a set of test visibility data and get the autocorrelations.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    VisibilityData visData(nAntennas, nChannels, nPols);
    std::vector<complex_t> autocorr(nAntennas * nChannels * nPols);

    // Fill the visibility matrix
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    std::complex<real_t> val( (ai+1)*(aj+1) );
                    visData(ai, aj, c, p) = val;
                }
            }
        }
    }

    // Get the autocorrelations.
    TIMER_START
    _basicFlagger->_getAutocorrelations(&visData, &autocorr[0]);
    TIMER_STOP("BasicFlagger::_getAutocorrelations (%d ant, %d chan, %d pol)",
            nAntennas, nChannels, nPols)

    // Check the autocorrelations.
    for (unsigned p = 0, i = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned a = 0; a < nAntennas; a++) {
                std::complex<real_t> val( (a+1)*(a+1) );
                CPPUNIT_ASSERT_EQUAL( val, autocorr[i] );
                i++;
            }
        }
    }
}

/**
 * @details
 * Tests the routine to get the median antenna power for each
 * channel and polarisation.
 */
void BasicFlaggerTest::test__getMedians()
{
    // Use Case
    // Create a set of test values and get the medians.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    std::vector<complex_t> autocorr(nAntennas * nChannels * nPols);
    std::vector<complex_t> medians(nChannels * nPols);

    // Fill the autocorrelation data.
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            /* Get the address of the start of the antenna list */
            const unsigned i = c * nAntennas + p * nAntennas * nChannels;
            complex_t* beg = &autocorr[i];
            complex_t* end = &autocorr[i + nAntennas];

            for (unsigned a = 0; a < nAntennas; a++) {
                std::complex<real_t> val( p + c + a*a );
                autocorr[i + a] = val;
            }
            std::random_shuffle(beg, end);
        }
    }

    // Get the medians.
    TIMER_START
    _basicFlagger->_getMedians(nAntennas, nChannels, nPols, &autocorr[0], &medians[0]);
    TIMER_STOP("BasicFlagger::_getMedians (%d ant, %d chan, %d pol)",
            nAntennas, nChannels, nPols)

    // Check the medians.
    for (unsigned p = 0, i = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            std::complex<real_t> val( p + c + (nAntennas/2)*(nAntennas/2) );
            CPPUNIT_ASSERT_EQUAL( val, medians[i] );
            i++;
        }
    }
}

/**
 * @details
 * Tests the routine to move the bad antennas to the edge of the visibility
 * matrix.
 */
void BasicFlaggerTest::test__moveBadAntennas()
{
    // Use Case
    // Create a set of test visibility data and get the autocorrelations.
    const unsigned nAntennas = 97;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    const real_t minFraction = 0.5;
    const real_t maxFraction = 1.5;
    VisibilityData visData(nAntennas, nChannels, nPols);
    FlagTable flagTable(nAntennas, nChannels, nPols);
    std::vector<complex_t> medians(nChannels * nPols);

    // Fill the visibility matrix
    for (unsigned p = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned aj = 0; aj < nAntennas; aj++) {
                for (unsigned ai = 0; ai < nAntennas; ai++) {
                    std::complex<real_t> val( sqrt((ai+1)*(aj+1)) );
                    visData(ai, aj, c, p) = val;
                }
            }
            medians[c + p * nChannels] = visData(nAntennas/2, nAntennas/2, c, p);
        }
    }

    // Flag the autocorrelations.
    _basicFlagger->_flagAutocorrelations(&visData, &medians[0],
            minFraction, maxFraction, &flagTable);

    // Move the bad antennas.
    TIMER_START
    _basicFlagger->_moveBadAntennas(&visData, &flagTable);
    TIMER_STOP("BasicFlagger::_moveBadAntennas (%d/%d ant in %d chan, %d pol)",
            flagTable.nFlaggedAntennas(0, 0), nAntennas, nChannels, nPols)
}

} // namespace pelican
