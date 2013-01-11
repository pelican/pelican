/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

/**
 * @file Vector3.h
 */

namespace pelican {

/**
 * @ingroup c_data
 *
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
        T x;
        T y;
        T z;

    public:
        /// Default constructor.
        inline Vector3() {}

        /// Construct a copy of vector \p a.
        inline Vector3(const Vector3 &a) {
            x = a.x; y = a.y; z = a.z;}

        /// Construct a copy of vector \p a.
        template<class T2> inline Vector3(const Vector3<T2> &a) {
                x = a.x; y = a.y; z = a.z;}

        /// Construct a vector with all elements set to the value \p a.
        inline Vector3(T a) {x = a; y = a; z = a;}

        /// Construct a vector with elements set to \p a \p b and \p c.
        inline Vector3(T a, T b, T c) {x = a; y = b; z = c;}

    public:
        /// Dot product.
        inline T dot(const Vector3<T> &b) const {
            return x * b.x + y * b.y + z * b.z;}

        /// Square of the vector length.
        inline T lengthSquared() const {
            return x * x + y * y + z * z;}

        /// Length of the vector.
        inline T length() const {return (T) sqrt(lengthSquared());}

        /// Angle between two vectors in radians.
        inline T angle(const Vector3<T> &b) const {
            return acos(dot(b)/sqrt(lengthSquared()*b.lengthSquared()));}

        /// Normalise the vector.
        inline void normalise() {
            T s = (T) (1.0 / length());
            x *= s;
            y *= s;
            z *= s;
        }

    public:
        /// Operator = (assignment).
        inline Vector3<T>& operator=(const Vector3 &a) {
            x = a.x; y = a.y; z = a.z;
            return *this;
        }

        /// Operator = (assignment).
        template<class T2> inline Vector3<T>& operator=(const Vector3<T2> &a) {
                x = a.x; y = a.y; z = a.z;
                return *this;
        }

        /// Operator == (test for equality).
        inline bool operator==(const Vector3<T> &a) const {
            return (x == a.x && y == a.y && z == a.z);}

        /// Operator != (test for inequality).
        inline bool operator!=(const Vector3<T> &a) const {
            return (x != a.x || y != a.y || z != a.z);}

        /// Operator += (additive assignment).
        inline Vector3<T>& operator+=(const Vector3<T> &a) {
            x += a.x; y += a.y; z += a.z;
            return *this;
        }

        /// Operator -= (subtractive assignment).
        inline Vector3<T>& operator-=(const Vector3<T> &a) {
            x -= a.x; y -= a.y; z -= a.z;
            return *this;
        }

        /// Operator *= (multiplicative scaling assignment).
        inline Vector3<T>& operator*=(T s) {
            x *= s; y *= s; z *= s;
            return *this;
        }

        /// Operator /= (fractional scaling assignment).
        inline Vector3<T>& operator/=(T s)  {
            x /= s; y /= s; z /= s;
            return *this;
        }

        /// Operator - (unary minus / negation).
        inline Vector3<T> operator-() const {
            return Vector3<T>(-x, -y, -z);}

        /// Operator + (unary plus).
        inline Vector3<T> operator+() const {return *this;}

        /// Operator + (addition).
        inline Vector3<T> operator+(const Vector3<T> &b) const {
            return Vector3<T>(x + b.x, y + b.y, z + b.z);}

        /// Operator - (subtraction).
        inline Vector3<T> operator-(const Vector3<T> &b) const  {
            return Vector3<T>(x - b.x, y - b.y, z - b.z);}

        /// Operator * (multiplicative scaling).
        inline Vector3<T> operator*(T s) const {
            return Vector3<T>(s * x, s * y, s * z);}

        /// Operator / (fractional scaling).
        inline Vector3<T> operator/(T s) const  {
            return Vector3<T>(x / s, y / s, z / s);}

        /// Operator ^ (cross product).
        inline Vector3<T> operator^(const Vector3<T> &b) const {
            return Vector3<T>(y * b.z - z * b.y,
                    z * b.x - x * b.z,
                    x * b.y - y * b.x);
        }
};

/// Typedef for Vector3<float>.
typedef Vector3<float> Vector3f;

/// Typedef for Vector3<double>.
typedef Vector3<double> Vector3d;

} // namespace pelican

#endif /* VECTOR3_H_ */
