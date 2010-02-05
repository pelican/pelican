#include "modules/BasicFlagger.h"
#include "data/VisibilityData.h"
#include <algorithm>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
BasicFlagger::BasicFlagger(const QDomElement& config)
    : AbstractModule(config)
{
    /* Set essential data requirements */
    addRemoteStreamData("VisibilityData");

    /* Parse configuration and add extra data requirements as needed */
    if (getOption("use_flag_table", "value").toLower() == "true") {
        _useFlagTable = true;
        addLocalServiceData("FlagTable");
    } else {
        _useFlagTable = false;
    }

    /* Get clipping levels */
    _minLevelMedian = getOption("min_level_median", "value", "0.25").toDouble();
    _maxLevelMedian = getOption("max_level_median", "value", "4.00").toDouble();
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
void BasicFlagger::run(QHash<QString, DataBlob*>& data)
{
    /* Get a pointer to the visibility data */
    VisibilityData *visibilityData = static_cast<VisibilityData*>(data.value("VisibilityData"));
    if (!visibilityData)
        throw QString("No visibility data.");

    /* Get the median autocorrelations */
    const unsigned nAntennas = visibilityData->nAntennas();
    const unsigned nChannels = visibilityData->nChannels();
    const unsigned nPols = visibilityData->nPolarisations();
    std::vector<complex_t> autocorr, medians;
    _getAutocorrelations(visibilityData, autocorr);
    _getMedians(nAntennas, nChannels, nPols, autocorr, medians);

    /* Loop through the autocorrelations and flag them if outside limits */
    // TODO
}

/**
 * @details
 * This method returns the autocorrelations of the antenna visibilities.
 *
 * The autocorrelations are returned in a standard vector, which is of length
 * nAntennas * nChannels * nPolarisations. The fastest-varying dimension is
 * the antenna data, and the slowest is the polarisation.
 *
 * @param[in] visData A pointer to the visibility data blob.
 * @param[in,out] autocorr A reference to the autocorrelation data.
 */
void BasicFlagger::_getAutocorrelations(const VisibilityData* visData,
        std::vector<complex_t>& autocorr)
{
    /* Ensure sufficient memory for the autocorrelations */
    const unsigned nPols = visData->nPolarisations();
    const unsigned nChannels = visData->nChannels();
    const unsigned nAntennas = visData->nAntennas();
    const unsigned nAntennasChannels = nAntennas * nChannels;
    const unsigned size = nPols * nAntennasChannels;
    if (size == 0) return;
    if (autocorr.size() != size)
        autocorr.resize(size);
    complex_t *aptr = &autocorr[0];

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
 * This method returns the median values of the autocorrelation data for
 * each channel and polarisation.
 *
 * The median antenna values are returned in a standard vector, which is of
 * length nChannels * nPolarisations. The fastest-varying dimension is
 * the channel index, and the slowest is the polarisation.
 *
 * The median is found optimally so that it runs in linear time, on average.
 *
 * The autocorrelation data is passed by value, since it is modified by the
 * function when obtaining the median.
 *
 * @param[in] nAntennas The number of antennas.
 * @param[in] nChannels The number of frequency channels.
 * @param[in] nPols The number of polarisations.
 * @param[in] autocorr The autocorrelation data (this must be copied).
 * @param[in,out] medians A reference to the median data.
 */
void BasicFlagger::_getMedians (
        const unsigned nAntennas,
        const unsigned nChannels,
        const unsigned nPols,
        std::vector<complex_t> autocorr,
        std::vector<complex_t>& medians
){
    /* Get start address of median data */
    const unsigned nAntennasChannels = nAntennas * nChannels;
    const unsigned size = nPols * nChannels;
    if (size == 0) return;
    if (medians.size() != size)
        medians.resize(size);
    complex_t *mptr = &medians[0];

    /* Loop over polarisations and channels */
    for (unsigned p = 0; p < nPols; ++p) {
        for (unsigned c = 0; c < nChannels; ++c) {
            /* Get pointer to autocorrelation data start and end */
            const unsigned i = c * nAntennas + p * nAntennasChannels;
            complex_t* aBeg = &autocorr[i];
            complex_t* aEnd = &autocorr[i + nAntennas];

            /* Find the median value by rearranging the autocorrelation
             * data. The cost is linear in the number of antennas. */
            const unsigned m = nAntennas / 2; // Median index.
            std::nth_element(aBeg, aBeg + m, aEnd, complexCompareAbs);

            /* Store median value */
            const unsigned j = c + p * nChannels;
            mptr[j] = aBeg[m];
        }
    }
}

/**
 * @details
 * Compares the absolute value (square modulus) of two complex numbers.
 */
bool complexCompareAbs(complex_t i, complex_t j) { return (abs(i) < abs(j)); }

} // namespace pelican
