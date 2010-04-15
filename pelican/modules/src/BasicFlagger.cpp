#include "pelican/modules/BasicFlagger.h"
#include "pelican/data/VisibilityData.h"
#include "pelican/data/FlagTable.h"
#include <algorithm>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_MODULE(BasicFlagger)

/**
 * @details
 * Module constructor.
 */
BasicFlagger::BasicFlagger(const ConfigNode& config)
    : AbstractModule(config)
{
    /* Get clipping levels */
    _minLevelMedian = config.getOption("min_level_median", "value", "0.25").toDouble();
    _maxLevelMedian = config.getOption("max_level_median", "value", "4.00").toDouble();
}

/**
 * @details
 * Module destructor.
 */
BasicFlagger::~BasicFlagger()
{
}

/**
 * @details
 * Runs the flagging module.
 *
 * @param[in,out] data The hash of data blobs required to run the pipeline
 *                     module. The module updates the contents of the data
 *                     blobs in the hash as required.
 */
void BasicFlagger::run(VisibilityData* visibilityData, FlagTable* flagTable)
{
    if (!visibilityData)
        throw QString("Visibility data not present.");

    if (!flagTable)
        throw QString("Flag table not present.");

    /* Get the dimension sizes */
    const unsigned nAntennas = visibilityData->nAntennas();
    const unsigned nChannels = visibilityData->nChannels();
    const unsigned nPols = visibilityData->nPolarisations();
    if (nAntennas == 0 || nChannels == 0 || nPols == 0)
        throw QString("Empty data set.");

    /* Get the median autocorrelations */
    std::vector<complex_t> autocorr(nAntennas * nChannels * nPols);
    std::vector<complex_t> medians(nChannels * nPols);
    _getAutocorrelations(visibilityData, &autocorr[0]);
    _getMedians(nAntennas, nChannels, nPols, &autocorr[0], &medians[0]);

    /* Flag autocorrelations if outside acceptable range */
    _flagAutocorrelations(visibilityData, &medians[0],
            _minLevelMedian, _maxLevelMedian, flagTable);

    /* Move bad antennas to the edge of the visibility matrix */
    _moveBadAntennas(visibilityData, flagTable);
}

/**
 * @details
 * This method flags the antenna autocorrelations based on the given median
 * autocorrelation values for each channel and polarisation. It writes entries
 * into the supplied flag table if the autocorrelation power is above or below
 * the given threshold.
 *
 * @param[in]     visData     A pointer to the visibility data blob.
 * @param[in]     medians     A pointer to the list of median values per channel & polarisation.
 * @param[in]     minFraction Minimum visibility level at which to flag, as a fraction of the median.
 * @param[in]     maxFraction Maximum visibility level at which to flag, as a fraction of the median.
 * @param[in,out] flagTable   A pointer to the flag table.
 */
void BasicFlagger::_flagAutocorrelations (
        const VisibilityData* visData,
        const complex_t* medians,
        const real_t minFraction,
        const real_t maxFraction,
        FlagTable* flagTable
){
    /* Get the dimension sizes */
    const unsigned nAntennas = visData->nAntennas();
    const unsigned nChannels = visData->nChannels();
    const unsigned nPols = visData->nPolarisations();

    /* Loop over polarisations and channels */
    for (unsigned p = 0; p < nPols; ++p) {
        for (unsigned c = 0; c < nChannels; ++c) {
            /* Get pointer to matrix start */
            const complex_t* mptr = visData->ptr(c, p);

            /* Get current median autocorrelation power */
            real_t medianPower = abs(medians[c + nChannels * p]);

            /* Check the diagonals */
            for (unsigned a = 0; a < nAntennas; ++a) {
                real_t antennaPower = abs(mptr[a + nAntennas * a]);
                if (antennaPower < medianPower * minFraction ||
                        antennaPower > medianPower * maxFraction) {
                    flagTable->flagAntenna(a, c, p, FlagTable::FLAG_AUTOCORR);
                }
            }
        }
    }
}

/**
 * @details
 * Given a data blob of type VisibilityData, this method returns the
 * autocorrelations of the antenna visibilities.
 *
 * The output autocorrelation array must be pre-allocated to length
 * nAntennas * nChannels * nPolarisations. The fastest-varying dimension is
 * the antenna data, and the slowest is the polarisation.
 *
 * @param[in]     visData A pointer to the visibility data blob.
 * @param[in,out] aptr    A pointer to the autocorrelation array to fill.
 */
void BasicFlagger::_getAutocorrelations(const VisibilityData* visData, complex_t* aptr)
{
    /* Get the dimension sizes */
    const unsigned nAntennas = visData->nAntennas();
    const unsigned nChannels = visData->nChannels();
    const unsigned nPols = visData->nPolarisations();
    const unsigned nAntennasChannels = nAntennas * nChannels;

    /* Loop over polarisations and channels */
    for (unsigned p = 0, i = 0; p < nPols; ++p) {
        for (unsigned c = 0; c < nChannels; ++c) {
            /* Get pointer to matrix start */
            const complex_t* mptr = visData->ptr(c, p);

            /* Copy the diagonals */
            for (unsigned a = 0; a < nAntennas; ++a) {
                i = a + c * nAntennas + p * nAntennasChannels;
                aptr[i] = mptr[a + nAntennas * a];
            }
        }
    }
}

/**
 * @details
 * This method returns the median values of the antenna autocorrelation data
 * for each channel and polarisation.
 *
 * The output median array must be pre-allocated to length
 * nChannels * nPolarisations. The fastest-varying dimension is
 * the channel index, and the slowest is the polarisation.
 *
 * The median is found optimally so that it runs in linear time, on average.
 *
 * @param[in] nAntennas The number of antennas.
 * @param[in] nChannels The number of frequency channels.
 * @param[in] nPols     The number of polarisations.
 * @param[in] aptr      A pointer to the autocorrelation array.
 * @param[in,out] mptr  A pointer to the array of median values to fill.
 */
void BasicFlagger::_getMedians (
        const unsigned nAntennas,
        const unsigned nChannels,
        const unsigned nPols,
        const complex_t* aptr,
        complex_t* mptr
){
    /* Get the dimension sizes */
    const unsigned nAntennasChannels = nAntennas * nChannels;
    const unsigned size = nAntennas * nChannels * nPols;
    if (size == 0) return;

    /* Create a working copy of the autocorrelation data */
    complex_t* autocorr = (complex_t*)malloc(size * sizeof(complex_t));
    memcpy(autocorr, aptr, size * sizeof(complex_t));

    /* Loop over polarisations and channels */
    for (unsigned p = 0; p < nPols; ++p) {
        for (unsigned c = 0; c < nChannels; ++c) {
            /* Get pointer to autocorrelation data start and end */
            const unsigned i = c * nAntennas + p * nAntennasChannels;
            complex_t* beg = autocorr + i;
            complex_t* end = autocorr + i + nAntennas;

            /* Find the median value by rearranging the autocorrelation
             * data. The cost is linear in the number of antennas. */
            const unsigned m = nAntennas / 2; // Median index.
            std::nth_element(beg, beg + m, end, complexCompareAbs);

            /* Store median value */
            const unsigned j = c + p * nChannels;
            mptr[j] = beg[m];
        }
    }

    /* Free the working copy */
    free(autocorr);
}

/**
 * @details
 * This method moves bad antennas (recorded as such in the flag table)
 * to the edge of the visibility matrix by swapping rows and columns.
 * The flag table data is also swapped so that it continues to mirror
 * the state of the visibility data.
 *
 * @param[in,out] visData   Pointer to the visibility data (the data is modified).
 * @param[in,out] flagTable Pointer to the flag table (the data is modified).
 */
void BasicFlagger::_moveBadAntennas (
        VisibilityData* visData,
        FlagTable* flagTable
){
    /* Get the dimension sizes */
    const unsigned nAntennas = visData->nAntennas();
    const unsigned nChannels = visData->nChannels();
    const unsigned nPols = visData->nPolarisations();

    /* Loop over polarisations and channels */
    for (unsigned p = 0; p < nPols; ++p) {
        for (unsigned c = 0; c < nChannels; ++c) {
            /* Reverse-sort the list of bad antennas so that we can GUARANTEE
             * that when we swap indices (starting from the highest bad
             * antenna), we don't interfere with anything else.
             * There's no way round this! */
            flagTable->reverseSortBadAntennas(c, p);

            /* Loop over bad antennas */
            unsigned nBadAntennas = flagTable->nFlaggedAntennas(c, p);
            for (unsigned iBad = 0; iBad < nBadAntennas; iBad++) {
                unsigned source = (flagTable->flaggedAntennas(c, p))[iBad];
                unsigned destin = nAntennas - 1 - iBad;
                visData->swapAntennaData(destin, source, c, p);
                flagTable->swapAntennaData(destin, source, c, p);
            }
        }
    }
}

/**
 * @details
 * Compares the absolute value (square modulus) of two complex numbers.
 */
bool complexCompareAbs(complex_t i, complex_t j) { return (abs(i) < abs(j)); }

} // namespace pelican
