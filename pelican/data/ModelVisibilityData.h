#ifndef MODELVISIBILITYDATA_H_
#define MODELVISIBILITYDATA_H_

#include "pelican/data/VisibilityData.h"
#include "pelican/data/Source.h"
#include <vector>

/**
 * @file ModelVisibilityData.h
 */

namespace pelican {

/**
 * @class ModelVisibilityData
 *
 * @brief
 * Container class to hold visibilities for a model sky positions.
 *
 * @details
 * Inherits the visibility data blob and adds information pertaining to the
 * list of sources (in RA and Dec) used to construct the visibility set.
 */
class ModelVisibilityData : public VisibilityData
{
    public:
        /// Constructs an empty model visibility data blob.
        ModelVisibilityData() : VisibilityData() {}

        /// Constructs and resizes model visibility data blob.
        ModelVisibilityData(const unsigned nAntennas,
                const std::vector<unsigned>& channels,
                const Polarisation polarisation)
        : VisibilityData(nAntennas, channels, polarisation) {}

        ~ModelVisibilityData() {}

    public:
        /// Returns vector of model sources.
        std::vector<Source>& sources() { return _sources; }

        /// Returns vector of model sources.
        const std::vector<Source>& sources() const { return _sources; }

    private:
        std::vector<Source> _sources;

};

} // namespace pelican

#endif // MODELVISIBILITYDATA_H_
