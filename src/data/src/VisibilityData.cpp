#include "data/VisibilityData.h"

namespace pelican {

/**
 * @details
 * Constructs a null set of visibility data.
 * The visibility matrix has zero size.
 */
VisibilityData::VisibilityData() : DataBlob()
{
    _init();
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
VisibilityData::VisibilityData(int antennas, int channels, int polarisations)
: DataBlob()
{
    _nAntennas = antennas;
    _nChannels = channels;
    _nPolarisations = polarisations;
    _visibilities.resize(antennas * antennas * channels * polarisations);
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
 * Initialises the class data members (called from constructors).
 */
void VisibilityData::_init()
{
    _nAntennas = 0;
    _nChannels = 0;
    _nPolarisations = 0;
}

} // namespace pelican
