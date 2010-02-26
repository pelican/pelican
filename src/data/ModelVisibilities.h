#ifndef MODELVISIBILITIES_H_
#define MODELVISIBILITIES_H_

#include "data/VisibilityData.h"
#include "data/Source.h"
#include <vector>

/**
 * @file ModelVisibilities.h
 */

namespace pelican {

/**
 * @class ModelVisibilities
 *
 * @brief
 * Container class to hold visibilities for a model sky positions.
 *
 * @details
 * Inherits the visibility data blob and adds information pertaining to the
 * list of sources (in RA and Dec) used to construct the visibility set.
 */
class ModelVisibilities : public VisibilityData
{
    public:
        /// Constructs and resizes model visibility data blob
        ModelVisibilities(const unsigned nAntennas = 0,
                const unsigned nChannels = 0,
                const unsigned nPolarisations = 0)
        : VisibilityData(nAntennas, nChannels, nPolarisations) {}

        ~ModelVisibilities() {}

    public:
        /// Returns vector of model sources.
        std::vector<Source>& sources() { return _sources; }

        /// Returns vector of model sources.
        const std::vector<Source>& sources() const { return _sources; }

    private:
        std::vector<Source> _sources;
};


} // namespace pelican

#endif /* MODELVISIBILITIES_H_ */
