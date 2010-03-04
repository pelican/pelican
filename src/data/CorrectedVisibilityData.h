#ifndef CORRECTEDVISIBILITYDATA_H_
#define CORRECTEDVISIBILITYDATA_H_

#include "data/VisibilityData.h"
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
        /// Constructs and resizes model visibility data blob
        CorrectedVisibilityData(const unsigned nAntennas = 0,
                const unsigned nChannels = 0,
                const unsigned nPolarisations = 0)
        : VisibilityData(nAntennas, nChannels, nPolarisations) {}

        ~CorrectedVisibilityData() {}
};


} // namespace pelican

#endif /* CORRECTEDVISIBILITYDATA_H_ */
