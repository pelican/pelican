#ifndef PELICANCONSTANTS_H_
#define PELICANCONSTANTS_H_

namespace pelican {

/**
 * @file constants.h
 */


/**
 * @brief
 * Constants used by pelican.
 *
 * @details
 */

namespace math {

const double pi = 3.141592653589793238462643383279502884197;
const double twoPi = 6.283185307179586476925286766559005768394;
const double asec2rad = 4.8481368110953599358991410E-6;
const double rad2deg = 57.2957795130823208767981548141051703;
const double deg2rad = 0.0174532925199432957692369076848861271;
const double asec2deg = 3600;

} // namespace math


namespace phy {

const double c = 299792458; // speed of light

} // namespace phy




} // namespace pelican

#endif /* PELICANCONSTANTS_H_ */
