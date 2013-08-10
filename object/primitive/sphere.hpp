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

#ifndef BACKTRACE_OBJECT_SPHERE_H
#define BACKTRACE_OBJECT_SPHERE_H

#include "../geometricobject.hpp"

namespace backtrace {

class Sphere : public GeometricObject
{
public:
    Sphere()
        : mRadius(1.0)
    {
    }

    Sphere(const Point3d& center, double radius)
        : mCenter(center),
        mRadius(radius)
    {
    }

    virtual ~Sphere() {}

    virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& result) const;

public: // TO-DO: Should be protected.
    Point3d mCenter;
    double mRadius;
};

bool Sphere::hit(const Ray& ray, double& tmin, ShadeRecord& result) const
{
    double      t;
    Vector3d    temp    = ray.origin - mCenter;
    double      a       = ray.direction * ray.direction;
    double      b       = 2.0 * temp * ray.direction;
    double      c       = temp * temp - mRadius * mRadius;
    double      disc    = b * b - 4.0 * a * c;
    
    if (disc < 0.0)
    {
        return false;
    }
    else
    {  
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom; // smaller root
    
        if (t > kEpsilon)
        {
            tmin = t;
            result.normal = (temp + t * ray.direction) / mRadius;
            result.localHitPoint = ray.origin + t * ray.direction;
            return true;
        } 
    
        t = (-b + e) / denom; // larger root
    
        if (t > kEpsilon)
        {
            tmin = t;
            result.normal = (temp + t * ray.direction) / mRadius;
            result.localHitPoint = ray.origin + t * ray.direction;
            return true;
        } 
    }
    
    return false;
}

}

#endif
