#include "pelican/modules/AstrometryFast.h"
#include "pelican/utility/constants.h"
#include <cmath>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This method performs the transformation from ICRS (Right Ascension and
 * Declination) to local horizon coordinates (azimuth and elevation) using
 * the site data and celestial data parameters.
 *
 * It calls private methods to correct for aberration, precession, nutation,
 * Earth rotation, site position, and atmospheric refraction. The
 * transformations should be accurate to less than two arcseconds.
 *
 * @param[in]  t   Populated structure containing site data.
 * @param[in]  c   Populated structure containing celestial data.
 * @param[in]  ra  The ICRS (J2000) Right Ascension in radians.
 * @param[in]  dec The ICRS (J2000) Declination in radians.
 * @param[out] a   The local azimuth in radians.
 * @param[out] e   The local elevation in radians.
 */
void AstrometryFast::icrsEquatorialToObserved(const SiteData* t,
        const CelestialData* c, double ra, double dec, double* a, double* e)
{
    // Convert to BCRS vector.
    double cosDec = cos(dec);
    Vector3d v;
    v.x = cos(ra) * cosDec;
    v.y = sin(ra) * cosDec;
    v.z = sin(dec);

    // Aberration (BCRS to GCRS).
    _aberrationVector(1.0, c->sol, &v);

    // Precession and nutation (GCRS to CIRS).
    _precessionNutationFromGcrs(c->pole, &v);

    // Earth rotation (CIRS to topocentric equatorial).
    _earthRotationFromCirs(c->ERA, &v);

    // Equatorial to horizon.
    _equatorialToHorizon(t->lat, &v);

    // Refraction.
    v.z += (2.77e-7 * t->pressure / v.z);

    // Renormalise.
    v.normalise();

    // Convert to angles.
    double w = atan2(v.y, -v.x);
    *a = (w >= 0) ? w : w + math::twoPi;
    *e = atan2(v.z, sqrt(v.x * v.x + v.y * v.y));
}

/**
 * @details
 * This method performs the transformation from local horizon coordinates
 * (azimuth and elevation) to ICRS (Right Ascension and Declination) using
 * the site data and celestial data parameters.
 *
 * It calls private methods to correct for aberration, precession, nutation,
 * Earth rotation, site position, and atmospheric refraction. The
 * transformations should be accurate to less than two arcseconds.
 *
 * @param[in] t   Populated structure containing site data.
 * @param[in] c   Populated structure containing celestial data.
 * @param[in] az  The local azimuth in radians.
 * @param[in] el  The local elevation in radians.
 * @param[out] r  The ICRS (J2000) Right Ascension in radians.
 * @param[out] d  The ICRS (J2000) Declination in radians.
 */
void AstrometryFast::icrsEquatorialFromObserved(const SiteData* t,
        const CelestialData* c, double az, double el, double* r, double* d)
{
    // Convert to vector (this is correct).
    az = az - math::pi;
    double cosEl = cos(el);
    Vector3d v;
    v.x = cos(az) * cosEl;
    v.y = -sin(az) * cosEl;
    v.z = sin(el);

    // Refraction.
    v.z -= (2.77e-7 * t->pressure / v.z);

    // Renormalise.
    v.normalise();

    // Horizon to equatorial.
    _equatorialFromHorizon(t->lat, &v);

    // Earth rotation (TIRS to CIRS).
    _earthRotationFromTirs(c->ERA, &v);

    // Precession and nutation (CIRS to GCRS).
    _precessionNutationFromCirs(c->pole, &v);

    // Aberration (GCRS to BCRS).
    _aberrationVector(-1.0, c->sol, &v);

    // Renormalise.
    v.normalise();

    // Convert to catalogue RA and Dec (this is correct).
    *r = atan2(v.y, v.x);
    *d = atan2(v.z, sqrt(v.x * v.x + v.y * v.y));
}

/**
 * @details
 * Populates the celestial parameters data structure, given the site data
 * and the current UT1.
 *
 * @param[in,out] c   Structure containing celestial parameter data.
 * @param[in]     t   Structure containing site parameters.
 * @param[in]     UT1 The UT1 as a Modified Julian Date.
 */
void AstrometryFast::setCelestialParameters(CelestialData* c,
        const SiteData* t, double UT1)
{
    // Timing.
    UT1 = _daysSinceJ2000(UT1);
    _earthRotationAngle(UT1, t->longitude, &c->ERA);
    _solarLongitude(UT1, &c->sol);

    // Coordinates of Celestial Intermediate Pole (CIP).
    _celestialIntermediatePoleCoords(UT1, &c->pole);
}

/**
 * @details
 * Adds the aberration vector to the given position, given the solar longitude.
 * The aberration vector is approximated as:
 *
 * \f{equation}{
 *    \textbf{V}_{A} \approx \left[\begin{array}{c}
 *                                  +0.99 \times 10^{-4} \sin \lambda \\
 *                                  -0.91 \times 10^{-4} \cos \lambda \\
 *                                  -0.40 \times 10^{-4} \cos \lambda \\
 *                            \end{array}\right]
 * \f}
 *
 * Reference: "Concise telescope pointing algorithm using IAU 2000 precepts",
 * by Patrick T. Wallace. (Bibcode 2008SPIE.7019E...7W, DOI 10.1117/12.788712)
 *
 * @param[in]     f       Factor by which to multiply the vector before addition.
 * @param[in]     lambda  Vector containing cosine and sine of solar longitude.
 * @param[in,out] v       The (updated) 3-vector.
 */
void AstrometryFast::_aberrationVector(double f, Vector2d lambda, Vector3d* v)
{
    v->x += f * (+0.99e-4 * lambda.sine);
    v->y += f * (-0.91e-4 * lambda.cosine);
    v->z += f * (-0.40e-4 * lambda.cosine);
}

/**
 * @details
 * Computes the coordinates of the Celestial Intermediate Pole (CIP), taking
 * into account first-order terms in precession and nutation.
 *
 * The CIP coordinates are computed as:
 *
 * \f{equation}{
 *    \textbf{V}_{CIP} \approx \left[\begin{array}{c}
 *                                  X_P + X_N \\
 *                                  Y_P + Y_N \\
 *                                  1 - X^2 / 2 \\
 *                             \end{array}\right]
 * \f}
 *
 * The precessional part of the motion is:
 *
 * \f{eqnarray}{
 *    X_P & \approx & +2.66 \times 10^{-7} t , \\
 *    Y_P & \approx & -8.14 \times 10^{-14} t^2 . \\
 * \f}
 *
 * Only the main 18.6-year nutation term is considered. Its argument is the
 * longitude of the Moon's ascending node; in radians:
 *
 * \f{equation}{
 *    \Omega \approx 2.182 - 9.242 \times 10^{-4} t.
 * \f}
 *
 * The nutation portion of the motion is:
 *
 * \f{eqnarray}{
 *    X_N & \approx & -33.2 \times 10^{-6} \sin\Omega , \\
 *    Y_N & \approx & 44.6 \times 10^{-6} \cos\Omega . \\
 * \f}
 *
 * Reference: "Concise telescope pointing algorithm using IAU 2000 precepts",
 * by Patrick T. Wallace. (Bibcode 2008SPIE.7019E...7W, DOI 10.1117/12.788712)
 *
 * @param[in] t    The (fractional) number of days elapsed since J2000.0.
 * @param[out] v   Coordinate vector of the Celestial Intermediate Pole.
 */
void AstrometryFast::_celestialIntermediatePoleCoords(double t, Vector3d* v)
{
    double omega = 2.182 - 9.242e-4 * t;
    v->x = 2.66e-7 * t + (-33.2e-6 * sin(omega));
    v->y = -8.14e-14 * t * t + 44.6e-6 * cos(omega);
    v->z = 1.0 - v->x * v->x / 2.0;
}

/**
 * @details
 * Given UT1 as a Modified Julian Date, computes and returns the number of days
 * since J2000.0 (given by \f$ UT1 - 51544.5 \f$).
 *
 * @param[in] ut The current UT1 as a Modified Julian Date (JD - 2400000.5).
 *
 * @return The number of (fractional) days since J2000.0.
 */
double AstrometryFast::_daysSinceJ2000(double ut)
{
    return ut - 51544.5;
}

/**
 * @details
 * Computes the current local Earth Rotation Angle (ERA) at the given
 * \p longitude.
 *
 * For longitude \f$\lambda\f$ (in radians), this is:
 *
 * \f{equation}{
 *     \theta = 4.8949612 + 6.300387486755 t + \lambda
 * \f}
 *
 * Reference: "Concise telescope pointing algorithm using IAU 2000 precepts",
 * by Patrick T. Wallace. (Bibcode 2008SPIE.7019E...7W, DOI 10.1117/12.788712)
 *
 * @param[in] t         The (fractional) number of days elapsed since J2000.0.
 * @param[in] longitude The observatory longitude in radians (east-positive).
 * @param[out] theta    Vector containing cosine and sine of local ERA.
 *
 * @return The local Earth Rotation Angle, in radians.
 */
double AstrometryFast::_earthRotationAngle(double t, double longitude, Vector2d* theta)
{
    double angle = 4.8949612 + 6.300387486755 * t + longitude;
    theta->cosine = cos(angle);
    theta->sine   = sin(angle);
    return angle;
}

/**
 * @details
 * Applies the Earth-rotation correction, going from the Celestial
 * Intermediate Reference System (CIRS) to the Terrestrial
 * Intermediate Reference System (TIRS) using the Earth Rotation Angle (ERA),
 * \f$ \theta \f$.
 *
 * The transformation is given by the following rotation about the z-axis:
 *
 * \f{equation}{
 *    \textbf{V}_{h, \delta} \approx \left[\begin{array}{c}
 *                                     x \cos \theta + y \sin \theta \\
 *                                     -x \sin \theta + y \cos \theta \\
 *                                     z \\
 *                                   \end{array}\right]
 * \f}
 *
 * The z-component of the vector is unchanged by the transformation.
 *
 * @param[in] theta  Vector containing cosine and sine of local ERA.
 * @param[in,out] v  The (input CIRS/output TIRS) components.
 */
void AstrometryFast::_earthRotationFromCirs(Vector2d theta, Vector3d* v)
{
    double xa = v->x, ya = v->y;
    v->x = xa * theta.cosine + ya * theta.sine;
    v->y = -xa * theta.sine + ya * theta.cosine;
}

/**
 * @details
 * Applies the Earth-rotation correction, going from the Terrestrial
 * Intermediate Reference System (TIRS) to the Celestial
 * Intermediate Reference System (CIRS) using the Earth Rotation Angle,
 * \f$ \theta \f$.
 *
 * The transformation is given by the following rotation about the z-axis:
 *
 * \f{equation}{
 *    \textbf{V}_{CIRS} \approx \left[\begin{array}{c}
 *                                     x \cos \theta - y \sin \theta \\
 *                                     x \sin \theta + y \cos \theta \\
 *                                     z \\
 *                                   \end{array}\right]
 * \f}
 *
 * The z-component of the vector is unchanged by the transformation.
 *
 * @param[in] theta  Vector containing cosine and sine of local ERA.
 * @param[in,out] x  The (input TIRS/output CIRS) x-component.
 * @param[in,out] y  The (input TIRS/output CIRS) y-component.
 */
void AstrometryFast::_earthRotationFromTirs(Vector2d theta, Vector3d* v)
{
    double xa = v->x, ya = v->y;
    v->x = xa * theta.cosine - ya * theta.sine;
    v->y = xa * theta.sine + ya * theta.cosine;
}

/**
 * @details
 * Apply the equatorial-to-horizon transformation using the site latitude.
 *
 * The transformation is given by the following rotation about the y-axis:
 *
 * \f{equation}{
 *    \textbf{V}_{TOPO} \approx \left[\begin{array}{c}
 *                                     x \sin \phi - z \cos \phi \\
 *                                     y \\
 *                                     x \cos \phi + z \sin \phi \\
 *                                   \end{array}\right]
 * \f}
 *
 * The y-component of the vector is unchanged by the transformation.
 *
 * @param[in] phi    Vector containing cosine and sine of site latitude.
 * @param[in,out] v  The (input equatorial/output horizontal) components.
 */
void AstrometryFast::_equatorialToHorizon(Vector2d phi, Vector3d* v)
{
    double xa = v->x, za = v->z;
    v->x = xa * phi.sine - za * phi.cosine;
    v->z = xa * phi.cosine + za * phi.sine;
}

/**
 * @details
 * Apply the horizon-to-equatorial transformation using the site latitude.
 *
 * The transformation is given by the following rotation about the y-axis:
 *
 * \f{equation}{
 *    \textbf{V}_{h, \delta} \approx \left[\begin{array}{c}
 *                                     x \sin \phi + z \cos \phi \\
 *                                     y \\
 *                                     -x \cos \phi + z \sin \phi \\
 *                                   \end{array}\right]
 * \f}
 *
 * The y-component of the vector is unchanged by the transformation.
 *
 * @param[in] phi    Vector containing cosine and sine of site latitude.
 * @param[in,out] v  The (input horizontal/output equatorial) components.
 */
void AstrometryFast::_equatorialFromHorizon(Vector2d phi, Vector3d* v)
{
    double xa = v->x, za = v->z;
    v->x = xa * phi.sine + za * phi.cosine;
    v->z = -xa * phi.cosine + za * phi.sine;
}

/**
 * @details
 * Applies the precession-nutation correction, going from the Geocentric
 * Celestial Reference System (GCRS, J2000) to Celestial Intermediate
 * Reference System (CIRS) of date.
 *
 * The transformation is
 * \f$ \textbf{v}_{CIRS} = \textbf{R}_{NPB} \cdot \textbf{v}_{GCRS} \f$,
 * and the rotation matrix can be approximated by:
 *
 * \f{equation}{
 *    \textbf{R}_{NPB} \approx \left[\begin{array}{ccc}
 *                                  1 - X^2 / 2 & 0 & -X \\
 *                                  0 & 1 & -Y \\
 *                                  X & Y & Z \\
 *                             \end{array}\right]
 * \f}
 *
 * Reference: "Concise telescope pointing algorithm using IAU 2000 precepts",
 * by Patrick T. Wallace. (Bibcode 2008SPIE.7019E...7W, DOI 10.1117/12.788712)
 *
 * @param[in] pole  The coordinates of the Celestial Intermediate Pole.
 * @param[in,out] v The (input GCRS/output CIRS) components.
 */
void AstrometryFast::_precessionNutationFromGcrs(Vector3d pole, Vector3d* v)
{
    double x = v->x, y = v->y, z = v->z;
    double X = pole.x, Y = pole.y, Z = pole.z;
    v->x = Z * x - X * z;
    v->y = y - Y * z;
    v->z = Z * z + X * x + Y * y;
}

/**
 * @details
 * Applies the precession-nutation correction, going from the Celestial
 * Intermediate Reference System (CIRS) to Geocentric Celestial Reference
 * System (GCRS, J2000).
 *
 * The transformation is
 * \f$ \textbf{v}_{GCRS} = \textbf{R}^{-1}_{NPB} \cdot \textbf{v}_{CIRS} \f$,
 * and the rotation matrix can be approximated by:
 *
 * \f{equation}{
 *    \textbf{R}^{-1}_{NPB} \approx \frac{1}{\left\vert \textbf{R} \right\vert}
 *                                  \left[\begin{array}{ccc}
 *                                      Z + Y^2 & -XY & X \\
 *                                      -XY & X^2 + Z^2 & YZ \\
 *                                      -X & -YZ & Z \\
 *                                  \end{array}\right]
 * \f}
 *
 * where \f$ \left\vert \textbf{R} \right\vert = X^2 + Z^2 + ZY^2 \f$.
 *
 * @param[in] pole  The coordinates of the Celestial Intermediate Pole.
 * @param[in,out] v The (input CIRS/output GCRS) components.
 */
void AstrometryFast::_precessionNutationFromCirs(Vector3d pole, Vector3d* v)
{
    double x = v->x, y = v->y, z = v->z;
    double X = pole.x, Y = pole.y, Z = pole.z;
    double det = Z*Z + X*X + Z*Y*Y;
    v->x = ( (Z + Y*Y) * x + (-X*Y) * y + (X) * z ) / det;
    v->y = ( (-X*Y) * x + (X*X + Z*Z) * y + (Y*Z) * z ) / det;
    v->z = ( (-X) * x + (-Y*Z) * y + (Z) * z ) / det;
}

/**
 * @details
 * Computes the solar longitude for the given time \p t:
 *
 * \f{equation}{
 *     \lambda \approx 4.895 + 1.72021 \times 10^{-2} t
 * \f}
 *
 * Reference: "Concise telescope pointing algorithm using IAU 2000 precepts",
 * by Patrick T. Wallace. (Bibcode 2008SPIE.7019E...7W, DOI 10.1117/12.788712)
 *
 * @param[in] t       The (fractional) number of days elapsed since J2000.0.
 * @param[out] lambda Vector containing cosine and sine of solar longitude.
 *
 * @return The solar longitude, in radians.
 */
double AstrometryFast::_solarLongitude(double t, Vector2d* lambda)
{
    double longitude = 4.895 + 1.72021e-2 * t;
    lambda->cosine = cos(longitude);
    lambda->sine   = sin(longitude);
    return longitude;
}

} // namespace pelican
