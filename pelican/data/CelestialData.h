#ifndef CELESTIALDATA_H_
#define CELESTIALDATA_H_

#include "pelican/data/Vector2.h"
#include "pelican/data/Vector3.h"

/**
 * @file CelestialData.h
 */

namespace pelican {

/**
 * @class CelestialData
 *
 * @brief
 * Celestial astrometric conversion parameters.
 *
 * @details
 * This class contains celestial data used for the astrometric
 * transformations.
 * - UT1 time as Modified Julian Date.
 * - Cosine and sine of Earth Rotation Angle.
 * - Cosine and sine of solar longitude.
 * - Coordinates of Celestial Intermediate Pole.
 *
 * All of these parameters are time-dependent.
 */
class CelestialData
{
    public:
        double UT1;        ///< UT1 time as Modified Julian Date.
        Vector2d ERA;      ///< Cosine and sine of Earth Rotation Angle.
        Vector2d sol;      ///< Cosine and sine of solar longitude.
        Vector3d pole;     ///< Coordinates of Celestial Intermediate Pole.
};

} // namespace pelican

#endif /* TERRESTRIALDATA_H_ */
