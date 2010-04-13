#ifndef ASTROMETRYFAST_H
#define ASTROMETRYFAST_H

#include "pelican/modules/AbstractAstrometry.h"
#include "pelican/data/CelestialData.h"
#include "pelican/data/SiteData.h"
#include "pelican/data/Vector2.h"
#include "pelican/data/Vector3.h"

/**
 * @file AstrometryFast.h
 */

namespace pelican {

/**
 * @class AstrometryFast
 *
 * @brief
 * Provides fast astrometric coordinate transformations accurate to
 * less than 2 arcsec.
 *
 * @details
 * This class implements the fast astrometric transformations described in
 * "Concise telescope pointing algorithm using IAU 2000 precepts",
 * by Patrick T. Wallace. (Bibcode 2008SPIE.7019E...7W, DOI 10.1117/12.788712)
 *
 * It provides routines to convert ICRS equatorial (Right Ascension and
 * Declination) to observed horizontal coordinates, and vice-versa, taking
 * into account:
 *
 * - Annual relativistic aberration.
 * - Precession.
 * - Nutation.
 * - Earth rotation.
 * - Site location.
 * - Atmospheric refraction.
 *
 * The effects neglected are:
 *
 * - Light deflection (under 2 arcsec even at Solar limb).
 * - Frame bias (negligible).
 * - Polar motion (this is below 0.5 arcsec).
 * - Diurnal aberration and parallax (this is below 0.3 arcsec).
 *
 * The approximations in these routines produce results that are accurate to
 * less than 2 arcsec for the whole of the 21st Century. The RMS error is less
 * than 1 arcsec.
 */
class AstrometryFast : public AbstractAstrometry
{
    public:
        /// Converts ICRS equatorial to observed horizontal coordinates.
        void icrsEquatorialToObserved(const SiteData* t,
                const CelestialData* c, double ra, double dec, double* a, double* e);

        /// Converts observed horizontal to ICRS equatorial coordinates.
        void icrsEquatorialFromObserved(const SiteData* t,
                const CelestialData* c, double az, double el, double* r, double* d);

        /// Populates the celestial parameters data structure.
        void setCelestialParameters(CelestialData* c, const SiteData* t,
                double UT1);

    private:
        /// Applies correction for relativistic aberration.
        void _aberrationVector(double f, Vector2d lambda, Vector3d* v);

        /// Computes the position of the Celestial Intermediate Pole.
        void _celestialIntermediatePoleCoords(double t, Vector3d* v);

        /// Computes the number of fractional days since epoch J2000.0.
        double _daysSinceJ2000(double ut);

        /// Computes the Earth Rotation Angle (ERA).
        double _earthRotationAngle(double t, double longitude, Vector2d* theta);

        /// Applies Earth rotation correction from CIRS to TIRS.
        void _earthRotationFromCirs(Vector2d theta, Vector3d* v);

        /// Applies Earth rotation correction from TIRS to CIRS.
        void _earthRotationFromTirs(Vector2d theta, Vector3d* v);

        /// Converts equatorial to horizon coordinates.
        void _equatorialToHorizon(Vector2d phi, Vector3d* v);

        /// Converts horizon to equatorial coordinates.
        void _equatorialFromHorizon(Vector2d phi, Vector3d* v);

        /// Applies precession and nutation correction from GCRS to CIRS.
        void _precessionNutationFromGcrs(Vector3d pole, Vector3d* v);

        /// Applies precession and nutation correction from CIRS to GCRS.
        void _precessionNutationFromCirs(Vector3d pole, Vector3d* v);

        /// Returns the solar longitude.
        double _solarLongitude(double t, Vector2d* lambda);
};

} // namespace pelican

#endif // ASTROMETRYFAST_H
