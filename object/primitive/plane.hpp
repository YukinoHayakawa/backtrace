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

#ifndef BACKTRACE_OBJECT_PLANE_H
#define BACKTRACE_OBJECT_PLANE_H

#include "../geometricobject.hpp"

namespace backtrace {

class Plane : public GeometricObject
{
public:
    Plane()
        : mNormal(0.0, 1.0, 0.0)
    {
    }

    Plane(const Point3d& point, const Normald& normal)
        : mPoint(point),
        mNormal(normal)
    {
        mNormal.normalize();
    }
    
    virtual ~Plane() {}
    
    virtual bool hit(Ray* ray, double& tmin, ShadeRecord* result) const;

public: // TO-DO: Should be protected.
    Point3d mPoint;
    Normald mNormal;
};

bool Plane::hit(Ray* ray, double& tmin, ShadeRecord* result) const
{
    double t = (mPoint - ray->origin) * mNormal / (ray->direction * mNormal);

    if(t > kEpsilon)
    {
        tmin = t;
        result->normal = mNormal;
        result->localHitPoint = ray->origin + t * ray->direction;
        return true;
    }
    else
    {
        return false;
    }
}

}

#endif
