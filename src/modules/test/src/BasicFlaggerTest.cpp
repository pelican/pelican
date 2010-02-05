#include "BasicFlaggerTest.h"
#include "modules/BasicFlagger.h"
#include "data/VisibilityData.h"
//#define TIMER_ENABLE
#include "utility/pelicanTimer.h"

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
}

/**
 * @details
 * Tests the run() method of the BasicFlagger using supplied
 * visibility data.
 */
void BasicFlaggerTest::test_run_withData()
{
    // Use Case
    // Create a set of test visibility data and pass it to the module.
    const unsigned nAntennas = 96;
    const unsigned nChannels = 512;
    const unsigned nPols = 2;
    const unsigned nTotal = nAntennas * nAntennas * nChannels * nPols;
    VisibilityData visData(nAntennas, nChannels, nPols);



    QHash<QString, DataBlob*> data;
    data.insert("VisibilityData", &visData);

    CPPUNIT_ASSERT_NO_THROW(_basicFlagger->run(data));
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
    std::vector<complex_t> autocorr;

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
    _basicFlagger->_getAutocorrelations(&visData, autocorr);
    TIMER_STOP("BasicFlagger::_getAutocorrelations (96 ant, 512 chan, 2 pol)")

    // Check the autocorrelations.
    CPPUNIT_ASSERT( autocorr.size() == (nAntennas * nChannels * nPols) );
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
    std::vector<complex_t> autocorr;
    std::vector<complex_t> medians;

    // Fill the autocorrelation data.
    autocorr.resize(nAntennas * nChannels * nPols);
    for (unsigned p = 0, i = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            for (unsigned a = 0; a < nAntennas; a++) {
                std::complex<real_t> val( p + c + a*a );
                autocorr[i] = val;
                i++;
            }
        }
    }

    // Get the medians.
    TIMER_START
    _basicFlagger->_getMedians(nAntennas, nChannels, nPols, autocorr, medians);
    TIMER_STOP("BasicFlagger::_getMedians (96 ant, 512 chan, 2 pol)")

    // Check the medians.
    CPPUNIT_ASSERT( medians.size() == (nChannels * nPols) );
    for (unsigned p = 0, i = 0; p < nPols; p++) {
        for (unsigned c = 0; c < nChannels; c++) {
            std::complex<real_t> val( p + c + (nAntennas/2)*(nAntennas/2) );
            CPPUNIT_ASSERT_EQUAL( val, medians[i] );
            i++;
        }
    }
}

} // namespace pelican
