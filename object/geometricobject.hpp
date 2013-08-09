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

#ifndef BACKTRACE_GEOMETRICOBJECT_H
#define BACKTRACE_GEOMETRICOBJECT_H

#include "../util/ray.hpp"
#include "../util/shaderecord.hpp"

namespace backtrace {

class GeometricObject
{
public:
    virtual ~GeometricObject() {};
    virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& result) const {};

    RGBColor getColor()
    {
        return mColor;
    }

    void setColor(const RGBColor& color)
    {
        mColor = color;
    }

protected:
    RGBColor mColor;
};

}

#endif
