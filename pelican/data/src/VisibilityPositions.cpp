#include "pelican/data/VisibilityPositions.h"

namespace pelican {

PELICAN_DECLARE_DATABLOB(VisibilityPositions)

/**
 * @details
 * Clears the visibility positions data blob.
 */
void VisibilityPositions::clear()
{
    _nAntennas = 0;
    _refChannel = 0;
    _refFreq = 0.0;
    _freqInc = 0.0;
    _u.clear();
    _v.clear();
    _w.clear();
}

/**
 * @details
 * Visibility positions are stored for a reference frequency therefore a
 * frequency dependent scaling factor is needed to convert the position from
 * the coordinates at one channel to another. This function returns
 * this scaling factor for a specified channel index.
 *
 * @param[in] channel Channel of the requested visibility scaling.
 *
 * @return Visibility position scaling factor.
 */
double VisibilityPositions::freqScaleFactor(const unsigned channel)
{
    return (_refFreq + (channel - _refChannel) * _freqInc) / _refFreq;
}

/**
 * @details
 * Assigns memory for an visibility positions data blob.
 *
 * @param[in]   nAntenna   The number of antennas.
 */
void VisibilityPositions::resize(const unsigned nAntennas)
{
   _refChannel = 0;
    _nAntennas = nAntennas;
    unsigned nBaselines = nAntennas * nAntennas;
   _u.resize(nBaselines);
   _v.resize(nBaselines);
   _w.resize(nBaselines);
}

} // namespace pelican
