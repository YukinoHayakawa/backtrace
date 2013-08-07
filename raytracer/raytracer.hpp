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

#ifndef BACKTRACE_RAYTRACERBASE_H
#define BACKTRACE_RAYTRACERBASE_H

#include "../util/ray.hpp"
#include "../util/rgbcolor.hpp"
#include "../scenemanager/scenemanager.hpp"
 
namespace backtrace {

class RayTracer
{
public:
    virtual ~RayTracer() {}

    virtual RGBColor traceRay(SceneManager* sm, Ray* ray)
    {
        return RGBColor();
    }
};

}

#endif
