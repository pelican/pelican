#ifndef VISIBILITYDATA_H
#define VISIBILITYDATA_H

#include "pelican/data/AntennaMatrixData.h"

/**
 * @file VisibilityData.h
 */

namespace pelican {

/**
 * @class VisibilityData
 *
 * @brief
 * Class to define telescope visibility data.
 *
 * @details
 * This class is used to store telescope visibility data
 * (matrices of complex correlation products).
 *
 * It is a typedef for AntennaMatrixData<complex_t>.
 */
class VisibilityData : public AntennaMatrixData<complex_t>
{
    public:
        /// Constructs an empty visibility data cube.
        VisibilityData() : AntennaMatrixData<complex_t>() {}

        /// Constructs an pre-sized visibility data cube.
        VisibilityData(const unsigned nAntennas,
                const std::vector<unsigned>& channels,
                const Polarisation polarisation)
        : AntennaMatrixData<complex_t>(nAntennas, channels, polarisation) {}

        /// Visibility data destructor.
        ~VisibilityData() {}
};

} // namespace pelican

#endif // VISIBILITYDATA_H
