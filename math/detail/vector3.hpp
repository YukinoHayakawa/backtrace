/*
 * Copyright 2013 TennenColl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BACKTRACE_MATH_DETAIL_VECTOR3_H
#define BACKTRACE_MATH_DETAIL_VECTOR3_H

#include <cmath>

namespace backtrace {

template<typename T>
struct Vector3
{
    T x, y, z;

    Vector3()
        : Vector3(0, 0, 0)
    {
    }

    Vector3(T a)
        : Vector3(a, a, a)
    {
    }

    Vector3(T x, T y, T z)
        : x(x), y(y), z(z)
    {
    }

    // As Vector3
    T mod2() const
    {
        return x * x + y * y + z * z;
    }

    // As Vector3
    T mod() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    // As Vector3
    T length2() const
    {
        return x * x + y * y + z * z;
    }

    // As Vector3
    T length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    // As Vector3 or Normal
    void normalize()
    {
        T invLength = 1 / sqrt(x * x + y * y + z * z);
        x *= invLength;
        y *= invLength;
        z *= invLength;
    }

    // As Point3
    T distance2(const Vector3<T>& to) const
    {
        return (
            (x - to.x) * (x - to.x) +
            (y - to.y) * (y - to.y) +
            (z - to.z) * (z - to.z)
        );
    }

    // As Point3
    T distance(const Vector3<T>& to) const
    {
        return sqrt(
            (x - to.x) * (x - to.x) +
            (y - to.y) * (y - to.y) +
            (z - to.z) * (z - to.z)
        );
    }

    Vector3 operator+(const Vector3& right) const
    {
        return Vector3(x + right.x, y + right.y, z + right.z);
    }

    Vector3& operator+=(const Vector3<T>& right)
    {
        x += right.x;
        y += right.y;
        z += right.z;

        return *this;
    }

    Vector3 operator-(const Vector3& right) const
    {
        return Vector3(x - right.x, y - right.y, z - right.z);
    }

    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator*(T scalar) const
    {
        return Vector3(scalar * x, scalar * y, scalar * z);
    }

    T operator*(const Vector3& another) const
    {
        return x * another.x + y * another.y + z * another.z;
    }

    Vector3 cross(const Vector3& right) const
    {
        return Vector3(
            y * right.z - z * right.y,
            z * right.x - x * right.z,
            x * right.y - y * right.x
        );
    }

    Vector3 operator/(T scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
};

template<typename T>
Vector3<T> operator*(T scalar, const Vector3<T>& vector)
{
    return Vector3<T>(scalar * vector.x, scalar * vector.y, scalar * vector.z);
}

}

#endif
