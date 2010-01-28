#include "data/AntennaPositionsData.h"

namespace pelican {

/**
 * @details
 * Constructs an empty antenna positions data blob
 */
AntennaPositions::AntennaPositions() : DataBlob()
{
}


/**
 * @details
 * Constructs and assigns memory for an antenna positions data blob.
 *
 * @param[in]   nAntennas   The number of antennas.
 */
AntennaPositions::AntennaPositions(int nAntennas)
: DataBlob()
{
    assign(nAntennas);
}


/**
 * @details
 * Image data destructor.
 */
AntennaPositions::~AntennaPositions()
{
}


/**
 * @details
 * Assigns memory for an antenna positions data blob.
 *
 * @param[in]   nAntenna   The number of antennas.
 */
void AntennaPositions::assign(int nAntennas)
{
   _x.resize(nAntennas);
   _y.resize(nAntennas);
   _z.resize(nAntennas);
}


/**
 * @details
 * Clears the antenna positions data blob.
 */
void ImageData::clear()
{
    _x.clear();
    _y.clear();
    _z.clear();
}


} // namespace pelican
