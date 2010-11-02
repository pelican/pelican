#ifndef VECTOR2_H_
#define VECTOR2_H_

/**
 * @file Vector2.h
 */

namespace pelican {

/**
 * @ingroup c_data
 *
 * @class Vector2
 *
 * @brief
 * Container class for a two-component vector.
 *
 * @details
 * Stores two components of a vector, or the sine and cosine of an angle.
 */
template<class T> class Vector2
{
    public:
        /// The vector data.
        union {
            T x;
            T cosine;
        };
        union {
            T y;
            T sine;
        };
};

/// Typedef for Vector2<float>.
typedef Vector2<float> Vector2f;

/// Typedef for Vector2<double>.
typedef Vector2<double> Vector2d;

} // namespace pelican

#endif /* VECTOR2_H_ */
