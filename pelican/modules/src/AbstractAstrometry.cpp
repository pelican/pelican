#include "pelican/modules/AbstractAstrometry.h"
#include "pelican/data/CelestialData.h"
#include "pelican/data/SiteData.h"
#include <cmath>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Sets the celestial parameter data structure required for coordinate
 * conversions.
 *
 * The default implementation simply sets UT1. The required Modified Julian Date
 * is given by JD - 2400000.5.
 *
 * @param[in,out] c  Updated celestial parameters.
 * @param[in] t      Pointer to populated terrestrial parameters.
 * @param[in] UT1    The UT1 time as a Modified Julian Date.
 */
void AbstractAstrometry::setCelestialParameters(CelestialData* c,
        const SiteData* /*t*/, double UT1)
{
    c->UT1 = UT1;
}

/**
 * @details
 * Populates the site parameters data structure, given the site longitude
 * and latitude and various atmospheric quantities (pressure, temperature,
 * humidity etc).
 *
 * This also stores delta UT, the UT1 - UTC offset.
 *
 * @param[in,out] t       Updated site parameters.
 * @param[in] longitude   Site geodetic longitude in radians.
 * @param[in] latitude    Site geodetic latitude in radians.
 * @param[in] deltaUT     The UT1 - UTC offset, in seconds.
 * @param[in] pressure    The atmospheric pressure in millibars (default 1000).
 * @param[in] temperature The air temperature in Kelvin (default 283).
 * @param[in] humidity    The normalised relative humidity (default 0.5).
 * @param[in] wavelength  The wavelength in microns (default 1e5).
 * @param[in] lapseRate   The tropospheric lapse rate in K/m (default 0.0065).
 * @param[in] altitude    Site altitude in metres (default 0).
 * @param[in] polarX      The x-component of polar motion (default 0).
 * @param[in] polarY      The y-component of polar motion (default 0).
 */
void AbstractAstrometry::setSiteParameters(SiteData* t, double longitude,
                double latitude, double deltaUT, double pressure,
                double temperature, double humidity,
                double wavelength, double lapseRate,
                double altitude, double polarX, double polarY)
{
    t->longitude = longitude;
    t->latitude = latitude;
    t->lat.cosine = cos(latitude);
    t->lat.sine = sin(latitude);
    t->deltaUT = deltaUT;
    t->pressure = pressure;
    t->temperature = temperature;
    t->humidity = humidity;
    t->wavelength = wavelength;
    t->lapseRate = lapseRate;
    t->altitude = altitude;
    t->polarMotion.x = polarX;
    t->polarMotion.y = polarY;
}

} // namespace pelican
