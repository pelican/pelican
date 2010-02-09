#include "data/VisibilityPositions.h"

namespace pelican {

/**
 * @details
 * Constructs an empty visibility positions data blob
 */
VisibilityPositions::VisibilityPositions() : DataBlob()
{
}


/**
 * @details
 * Constructs and assigns memory for an visibility positions data blob.
 *
 * @param[in]   nAntennas   The number of antennas.
 */
VisibilityPositions::VisibilityPositions(const unsigned nAntennas)
: DataBlob()
{
    assign(nAntennas);
}


/**
 * @details
 * Image data destructor.
 */
VisibilityPositions::~VisibilityPositions()
{
}


/**
 * @details
 * Assigns memory for an visibility positions data blob.
 *
 * @param[in]   nAntenna   The number of antennas.
 */
void VisibilityPositions::assign(const unsigned nAntennas)
{
   _refChannel = 0;
    _nAntennas = nAntennas;
    unsigned nBaselines = nAntennas * nAntennas;
   _u.resize(nBaselines);
   _v.resize(nBaselines);
   _w.resize(nBaselines);
}


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

} // namespace pelican
