#ifndef FLAGTABLE_H
#define FLAGTABLE_H

#include "data/AntennaMatrixData.h"
#include <vector>

/**
 * @file FlagTable.h
 */

namespace pelican {

/**
 * @class FlagTable
 *  
 * @brief
 * Holds flag table data.
 * 
 * @details
 * This class holds flag table data. It inherits
 * the AntennaMatrixData<unsigned char> base class, which is used to define
 * the storage format.
 */
class FlagTable : public AntennaMatrixData<unsigned char>
{
    public:
        /// Enumerator describing the flag reason code.
        typedef enum {
            FLAG_STOKES_USE = 0x1,
            FLAG_RFI_BAD = 0x2,
            FLAG_UVDIST = 0x4,
            FLAG_AUTOCORR = 0x8
        } reason_t;

        /// Constructs an empty data cube.
        /// The constructed data cube has zero size.
        ///
        /// @param[in] use2dAntennaMatrix If set, use a 2D antenna matrix (default true).
        FlagTable(bool use2dAntennaMatrix = true) :
            AntennaMatrixData<unsigned char>(use2dAntennaMatrix) {}

        /// Constructs a pre-sized, empty cube.
        /// The cube is pre-sized using the given parameters.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        /// @param[in] use2dAntennaMatrix If set, use a 2D antenna matrix (default true).
        FlagTable(unsigned antennas, unsigned channels,
                unsigned polarisations, bool use2dAntennaMatrix = true) :
                    AntennaMatrixData<unsigned char>(antennas, channels, polarisations, use2dAntennaMatrix) {}

        /// Destructor.
        ~FlagTable() {}
};

} // namespace pelican

#endif // FLAGTABLE_H
