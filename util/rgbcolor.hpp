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

#ifndef BACKTRACE_UTIL_RGBCOLOR_H
#define BACKTRACE_UTIL_RGBCOLOR_H

#include <cmath>

namespace backtrace {

class RGBColor
{
public:
    float r;
    float g;
    float b;

public:
    RGBColor()
        : r(0.0f), g(0.0f), b(0.0f)
    {
    }

    RGBColor(float color)
        : r(color), g(color), b(color)
    {
    }

    RGBColor(float r, float g, float b)
        : r(r), g(g), b(b)
    {
    }

    RGBColor operator+(const RGBColor& another) const
    {
        return RGBColor(r + another.r, g + another.g, b + another.b);
    }

    RGBColor& operator+=(const RGBColor& another)
    {
        r += another.r;
        g += another.g;
        b += another.b;

        return *this;
    }

    RGBColor operator*(const float scalar) const
    {
        return RGBColor(r * scalar, g * scalar, b * scalar);
    }

    RGBColor& operator*=(const float scalar)
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;

        return *this;
    }

    RGBColor operator/(const float scalar) const
    {
        return RGBColor(r / scalar, g / scalar, b / scalar);
    }

    RGBColor& operator/=(const float scalar)
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;

        return *this;
    }

    RGBColor operator*(const RGBColor& another) const
    {
        return RGBColor(r * another.r, g * another.g, b * another.b);
    }

    RGBColor powc(float power) const
    {
        return RGBColor(pow(r, power), pow(g, power), pow(b, power));
    }

    float average() const
    {
        return 0.333333333333 * (r + g + b);
    }
};

RGBColor operator*(const float scalar, const RGBColor& color)
{
    return RGBColor(color.r * scalar, color.g * scalar, color.b * scalar);
}

}

#endif
