#ifndef ANTENNAGAINS_H
#define ANTENNAGAINS_H

#include "data/AntennaMatrixData.h"

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
 * This class is used to store complex gains (amplitude and phase)
 * for each antenna.
 *
 * It is a typedef for AntennaMatrixData<complex_t>.
 */
typedef AntennaMatrixData<complex_t> AntennaGains;

} // namespace pelican

#endif // ANTENNAGAINS_H
