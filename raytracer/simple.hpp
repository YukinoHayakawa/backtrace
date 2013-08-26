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

#ifndef BACKTRACE_RAYTRACER_SIMPLE_H
#define BACKTRACE_RAYTRACER_SIMPLE_H

#include "../util/ray.hpp"
#include "../scene/scene.hpp"
#include "../util/shaderecord.hpp"

#include "raytracer.hpp"
 
namespace backtrace {

class SimpleRayTracer : public RayTracer
{
public:
    virtual ~SimpleRayTracer() {}

    virtual RGBColor traceRay(Ray* ray, Scene* scene)
    {
        ShadeRecord rec;
        scene->hitObject(ray, &rec);
        if(rec.hitAnObject)
            return rec.color;
        else
            return RGBColor();
    }
};

}

#endif
