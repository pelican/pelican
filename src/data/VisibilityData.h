#ifndef VISIBILITYDATA_H
#define VISIBILITYDATA_H

#include "data/AntennaMatrixData.h"

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
typedef AntennaMatrixData<complex_t> VisibilityData;

} // namespace pelican

#endif // VISIBILITYDATA_H
