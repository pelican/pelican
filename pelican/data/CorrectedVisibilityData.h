#ifndef CORRECTEDVISIBILITYDATA_H_
#define CORRECTEDVISIBILITYDATA_H_

#include "pelican/data/VisibilityData.h"
#include <vector>

/**
 * @file CorrectedVisibilityData.h
 */

namespace pelican {

/**
 * @class CorrectedVisibilityData
 *
 * @brief
 * Container class to hold corrected visibilities.
 *
 * @details
 */
class CorrectedVisibilityData : public VisibilityData
{
    public:
        /// Constructs an empty corrected visibility data blob.
        CorrectedVisibilityData() : VisibilityData() {}


        /// Constructs and resizes corrected visibility data blob.
        CorrectedVisibilityData(const unsigned nAntennas,
                const std::vector<unsigned>& channels,
                const Polarisation polarisation)
        : VisibilityData(nAntennas, channels, polarisation) {}

        ~CorrectedVisibilityData() {}
};

} // namespace pelican

#endif // CORRECTEDVISIBILITYDATA_H_
