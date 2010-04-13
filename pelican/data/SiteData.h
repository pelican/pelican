#ifndef SITEDATA_H_
#define SITEDATA_H_

#include "pelican/data/Vector2.h"

/**
 * @file SiteData.h
 */

namespace pelican {

/**
 * @class SiteData
 *
 * @brief
 * Terrestrial (site-based) astrometric conversion parameters.
 *
 * @details
 * This class contains site-based (constant or slowly time varying)
 * terrestrial data used for the astrometric transformations.
 *
 * - Site geodetic longitude in radians (east-positive).
 * - Site geodetic latitude in radians.
 * - Cosine and sine of site geodetic latitude.
 * - The UT1 - UTC offset, in seconds.
 * - Atmospheric pressure in millibars.
 * - Site altitude above sea level in metres.
 * - Atmospheric temperature in Kelvin.
 * - Normalised relative humidity.
 * - Wavelength of observation in microns.
 * - Tropospheric lapse rate in Kelvin/metre.
 * - The x- and y-components of polar motion.
 */
class SiteData
{
    public:
        double longitude;     ///< Site geodetic longitude in radians.
        double latitude;      ///< Site geodetic latitude in radians.
        Vector2d lat;         ///< Cosine and sine of site geodetic latitude.
        double deltaUT;       ///< The UT1 - UTC offset, in seconds.
        double pressure;      ///< Atmospheric pressure in millibars.
        double temperature;   ///< Atmospheric temperature in Kelvin.
        double humidity;      ///< Normalised relative humidity.
        double wavelength;    ///< Wavelength of observation in microns.
        double lapseRate;     ///< Tropospheric lapse rate in Kelvin/metre.
        double altitude;      ///< Site altitude above sea level in metres.
        Vector2d polarMotion; ///< The x- and y-components of polar motion.
};

} // namespace pelican

#endif /* SITEDATA_H_ */
