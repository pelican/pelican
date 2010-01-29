#ifndef VISIBILITYDATA_H
#define VISIBILITYDATA_H

#include "data/MatrixData.h"

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
 * It is a typedef for MatrixData<complex_t>.
 */
typedef MatrixData<complex_t> VisibilityData;

} // namespace pelican

#endif // VISIBILITYDATA_H
