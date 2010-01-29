#ifndef ANTENNAGAINS_H
#define ANTENNAGAINS_H

#include "data/MatrixData.h"

/**
 * @file AntennaGains.h
 */

namespace pelican {

/**
 * @class AntennaGains
 *
 * @brief
 * Class to store antenna complex gain data.
 * 
 * @details
 * This class is used to store complex antenna gains for each
 * (matrices of complex correlation products).
 *
 * It is a typedef for MatrixData<complex_t>.
 */
typedef MatrixData<complex_t> AntennaGains;

} // namespace pelican

#endif // ANTENNAGAINS_H
