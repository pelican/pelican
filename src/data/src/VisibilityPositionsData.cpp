#include "data/VisibilityPositionsData.h"

namespace pelican {

/**
 * @details
 * Constructs an empty visibility positions data blob
 */
VisibilityPositionsData::VisibilityPositionsData() : DataBlob()
{
}


/**
 * @details
 * Constructs and assigns memory for an visibility positions data blob.
 *
 * @param[in]   nAntennas   The number of antennas.
 */
VisibilityPositionsData::VisibilityPositionsData(const unsigned nAntennas)
: DataBlob()
{
    assign(nAntennas);
}


/**
 * @details
 * Image data destructor.
 */
VisibilityPositionsData::~VisibilityPositionsData()
{
}


/**
 * @details
 * Assigns memory for an visibility positions data blob.
 *
 * @param[in]   nAntenna   The number of antennas.
 */
void VisibilityPositionsData::assign(const unsigned nAntennas)
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
void VisibilityPositionsData::clear()
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
double VisibilityPositionsData::freqScaleFactor(const unsigned channel)
{
    return (_refFreq + (channel - _refChannel) * _freqInc) / _refFreq;
}

} // namespace pelican
