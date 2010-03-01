#ifndef VECTOR3_H_
#define VECTOR3_H_

/**
 * @file Vector3.h
 */

namespace pelican {

/**
 * @class Vector3
 *
 * @brief
 * Container class for a three-component vector.
 *
 * @details
 * Stores three components of a cartesian vector.
 */
template<class T> class Vector3
{
    public:
        /// The vector data.
        union {
            T v[3];
            struct {
                    T x;
                    T y;
                    T z;
            };
        };

    public:
        /// Default constructor.
        inline Vector3() {}

        /// Construct a copy of vector \p a.
        inline Vector3(const Vector3 &a) {
            v[0] = a.v[0]; v[1] = a.v[1]; v[2] = a.v[2];}

        /// Construct a vector from the memory at \p b.
        inline Vector3(const T *b) {
            v[0] = b[0]; v[1] = b[1]; v[2] = b[2];}

        /// Construct a copy of vector \p a.
        template<class T2> inline Vector3(const Vector3<T2> &a) {
                v[0] = a.v[0]; v[1] = a.v[1]; v[2] = a.v[2];}

        /// Construct a vector with all elements set to the value \p x.
        inline Vector3(T x) {v[0] = x; v[1] = x; v[2] = x;}

        /// Construct a vector with elements set to \p x \p y and \p z.
        inline Vector3(T x, T y, T z) {v[0] = x; v[1] = y; v[2] = z;}

    public:
        /// Sets the contents of the vector to \p x \p y and \p z.
        inline void set(T x, T y, T z) {v[0] = x; v[1] = y; v[2] = z;}

        /// Dot product.
        inline T dot(const Vector3<T> &b) const {
            return v[0] * b.v[0] + v[1] * b.v[1] + v[2] * b.v[2];}

        /// Square of the vector length.
        inline T lengthSquared() const {
            return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];}

        /// Length of the vector.
        inline T length() const {return (T) sqrt(lengthSquared());}

        /// Angle between two vectors in radians.
        inline T angle(const Vector3<T> &b) const {
            return acos(dot(b)/sqrt(lengthSquared()*b.lengthSquared()));}

        /// Normalise the vector.
        inline void normalise() {
            T s = (T) (1.0 / length());
            v[0] *= s;
            v[1] *= s;
            v[2] *= s;
        }

    public:
        /// Operator = (assignment).
        inline Vector3<T>& operator=(const Vector3 &a) {
            v[0] = a.v[0]; v[1] = a.v[1]; v[2] = a.v[2];
            return *this;
        }

        /// Operator = (assignment).
        template<class T2> inline Vector3<T>& operator=(const Vector3<T2> &a) {
                v[0] = a.v[0]; v[1] = a.v[1]; v[2] = a.v[2];
                return *this;
        }

        /// Operator == (test for equality).
        inline bool operator==(const Vector3<T> &a) const {
            return (v[0] == a.v[0] && v[1] == a.v[1] && v[2] == a.v[2]);}

        /// Operator != (test for inequality).
        inline bool operator!=(const Vector3<T> &a) const {
            return (v[0] != a.v[0] || v[1] != a.v[1] || v[2] != a.v[2]);}

        /// Operator [] (array reference).
        inline T& operator[](int i) {return v[i];}

        /// Operator [] (constant array reference).
        inline const T& operator[](int i) const {return v[i];}

        /// Indirection (dereference).
        inline operator T*() {return v;}

        /// Indirection (dereference).
        inline operator const T*() const {return v;}

        /// Operator += (additive assignment).
        inline Vector3<T>& operator+=(const Vector3<T> &a) {
            v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2];
            return *this;
        }

        /// Operator -= (subtractive assignment).
        inline Vector3<T>& operator-=(const Vector3<T> &a) {
            v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2];
            return *this;
        }

        /// Operator *= (multiplicative scaling assignment).
        inline Vector3<T>& operator*=(T s) {
            v[0] *= s; v[1] *= s; v[2] *= s;
            return *this;
        }

        /// Operator /= (fractional scaling assignment).
        inline Vector3<T>& operator/=(T s)  {
            v[0] /= s; v[1] /= s; v[2] /= s;
            return *this;
        }

        /// Operator - (unary minus / negation).
        inline Vector3<T> operator-() const {
            return Vector3<T>(-v[0], -v[1], -v[2]);}

        /// Operator + (unary plus).
        inline Vector3<T> operator+() const {return *this;}

        /// Operator + (addition).
        inline Vector3<T> operator+(const Vector3<T> &b) const {
            return Vector3<T>(v[0] + b.v[0], v[1] + b.v[1], v[2] + b.v[2]);}

        /// Operator - (subtraction).
        inline Vector3<T> operator-(const Vector3<T> &b) const  {
            return Vector3<T>(v[0] - b.v[0], v[1] - b.v[1], v[2] - b.v[2]);}

        /// Operator * (multiplicative scaling).
        inline Vector3<T> operator*(T s) const {
            return Vector3<T>(s * v[0], s * v[1], s * v[2]);}

        /// Operator / (fractional scaling).
        inline Vector3<T> operator/(T s) const  {
            return Vector3<T>(v[0] / s, v[1] / s, v[2] / s);}

        /// Operator ^ (cross product).
        inline Vector3<T> operator^(const Vector3<T> &b) const {
            return Vector3<T>(v[1] * b.v[2] - v[2] * b.v[1],
                    v[2] * b.v[0] - v[0] * b.v[2],
                    v[0] * b.v[1] - v[1] * b.v[0]);
        }
};

/// Typedef for Vector3<float>.
typedef Vector3<float> Vector3f;

/// Typedef for Vector3<double>.
typedef Vector3<double> Vector3d;

} // namespace pelican

#endif /* VECTOR3_H_ */
