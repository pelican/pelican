#include "data/VisibilityData.h"

namespace pelican {

/**
 * @details
 * Constructs a null set of visibility data.
 * The visibility matrix has zero size.
 */
VisibilityData::VisibilityData() : DataBlob()
{
    resize(0, 0, 0);
}

/**
 * @details
 * Constructs an empty set of visibility data.
 * The visibility matrix is pre-sized to the given parameters.
 *
 * @param[in] antennas The number of antennas in the visibility matrix.
 * @param[in] channels The number of frequency channels.
 * @param[in] polarisations The number of polarisations.
 */
VisibilityData::VisibilityData(unsigned antennas,
        unsigned channels, unsigned polarisations)
: DataBlob()
{
    resize(antennas, channels, polarisations);
}

/**
 * @details
 * Visibility data destructor.
 */
VisibilityData::~VisibilityData()
{
}

/**
 * @details
 * This function resizes the storage allocated for the visibility data.
 *
 * @param[in] antennas The number of antennas in the visibility matrix.
 * @param[in] channels The number of frequency channels.
 * @param[in] polarisations The number of polarisations.
 */
void VisibilityData::resize(unsigned antennas,
        unsigned channels, unsigned polarisations)
{
    _nAntennas = antennas;
    _nChannels = channels;
    _nPolarisations = polarisations;
    _visibilities.resize(antennas * antennas * channels * polarisations);
}

} // namespace pelican
