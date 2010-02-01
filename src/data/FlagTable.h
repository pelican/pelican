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
 * the AntennaMatrixData<char> base class, which is used to define
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

        /// Default constructor.
        FlagTable() : AntennaMatrixData<unsigned char>() {}

        /// Destructor.
        ~FlagTable();
};

} // namespace pelican

#endif // FLAGTABLE_H
