#ifndef ABSTRACTASTROMETRY_H
#define ABSTRACTASTROMETRY_H

/**
 * @file AbstractAstrometry.h
 */

namespace pelican {

class SiteData;
class CelestialData;

/**
 * @class AbstractAstrometry
 *  
 * @brief
 * Provides an interface for astrometric transformations.
 * 
 * @details
 * This class provides interfaces for astrometric coordinate transformations.
 * Call setSiteParameters() and setCelestialParameters() to populate the
 * required data structures.
 *
 * Sub-classes implement the functionality to convert between coordinate systems.
 * To convert ICRS equatorial (loosely, J2000) to observed horizontal
 * coordinates, call icrsEquatorialToObserved(). For the inverse
 * transformation, observed horizontal to ICRS equatorial, call
 * icrsEquatorialFromObserved().
 */
class AbstractAstrometry
{
    public:
        /// Converts ICRS equatorial to observed horizontal coordinates.
        virtual void icrsEquatorialToObserved(const SiteData* t,
                const CelestialData* c, double ra, double dec,
                double* a, double* e) = 0;

        /// Converts observed horizontal to ICRS equatorial coordinates.
        virtual void icrsEquatorialFromObserved(const SiteData* t,
                const CelestialData* c, double az, double el,
                double* r, double* d) = 0;

        /// Populates the celestial parameters data structure.
        virtual void setCelestialParameters(CelestialData* c, const SiteData* t,
                double UT1);

        /// Populates the site parameters data structure.
        void setSiteParameters(SiteData* t, double longitude,
                double latitude, double deltaUT, double pressure = 1000,
                double temperature = 283, double humidity = 0.5,
                double wavelength = 1e5, double lapseRate = 0.0065,
                double altitude = 0, double polarMotionX = 0,
                double polarMotionY = 0);
};

} // namespace pelican

#endif // ABSTRACTASTROMETRY_H 
