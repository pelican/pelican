#include "pelican/data/AntennaPositions.h"

namespace pelican {

PELICAN_DECLARE_DATABLOB(AntennaPositions)

/**
 * @details
 * Clears the antenna positions data blob.
 */
void AntennaPositions::clear()
{
    _x.clear();
    _y.clear();
    _z.clear();
}

/**
 * @details
 * Assigns memory for an antenna positions data blob.
 *
 * @param[in]   nAntenna   The number of antennas.
 */
void AntennaPositions::resize(const unsigned int nAntennas)
{
   _x.resize(nAntennas);
   _y.resize(nAntennas);
   _z.resize(nAntennas);
}

} // namespace pelican
