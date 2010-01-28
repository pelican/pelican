#include "data/VisibilityPositionsData.h"

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
VisibilityPositions::VisibilityPositions(int nAntennas)
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
void VisibilityPositions::assign(int nAntennas)
{
   _nAntennas = nAntennas;
    int nBaselines = nAntennas * nAntennas;
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


} // namespace pelican
