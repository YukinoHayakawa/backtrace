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

#ifndef BACKTRACE_RAYTRACER_ROOT_H
#define BACKTRACE_RAYTRACER_ROOT_H

#include <memory>

#include "event/event.hpp"
#include "object/geometricobject.hpp"
#include "math/math.hpp"
#include "raytracer/raytracer.hpp"
#include "rendertarget/rendertarget.hpp"
#include "scenemanager/scenemanager.hpp"

namespace backtrace {

class Root
{
public:
    std::unique_ptr<SceneManager> sceneManager;
    std::unique_ptr<RenderTarget> renderTarget;
    std::unique_ptr<RayTracer> rayTracer;
    RGBColor backgroundColor;

    float pixelSize;

public:
    Root(SceneManager* sceneManager,
        RenderTarget* renderTarget,
        RayTracer* rayTracer)
        : sceneManager(sceneManager),
        renderTarget(renderTarget),
        rayTracer(rayTracer),
        pixelSize(0.01)
    {
    }

    virtual ~Root() {}

    virtual void renderScene()
    {
        RGBColor pixelColor;
        Ray ray;
        double zw = 100.0;
        double x, y;

        ray.direction = Vector3d(0.0, 0.0, -1.0);

        for(int r = 0; r < renderTarget->getHeight(); ++r)
        {
            for(int c = 0; c < renderTarget->getWidth(); ++c)
            {
                x = pixelSize * (c - 0.5 * (renderTarget->getWidth() - 1.0));
                y = pixelSize * (r - 0.5 * (renderTarget->getHeight() - 1.0));
                ray.origin = Point3d(x, y, zw);
                pixelColor = rayTracer->traceRay(sceneManager.get(), &ray);
                renderTarget->drawPixel(c, r, pixelColor.r * 255, pixelColor.g * 255, pixelColor.b * 255);
            }
        }
    }
};

}

#endif
