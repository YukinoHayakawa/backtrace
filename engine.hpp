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
#include "sampler/sampler.hpp"

namespace backtrace {

class Engine
{
public:
    std::unique_ptr<SceneManager> sceneManager;
    std::unique_ptr<RenderTarget> renderTarget;
    std::unique_ptr<RayTracer> rayTracer;
    std::unique_ptr<Sampler> sampler;
    RGBColor backgroundColor;

    float pixelSize;

public:
    Engine(SceneManager* sceneManager,
        RenderTarget* renderTarget,
        RayTracer* rayTracer,
        Sampler* sampler)
        : sceneManager(sceneManager),
        renderTarget(renderTarget),
        rayTracer(rayTracer),
        sampler(sampler),
        pixelSize(0.01)
    {
        sampler->generateSamples();
    }

    virtual ~Engine() {}

    virtual void renderSceneOrthographic()
    {
        RGBColor pixelColor;
        Ray ray;
        double zw = 100.0;
        Point2d unitSample;
        Point2d pixelSample;

        ray.direction = Vector3d(0.0, 0.0, -1.0);

        for(int r = 0; r < renderTarget->getHeight(); ++r)
        {
            for(int c = 0; c < renderTarget->getWidth(); ++c)
            {
                pixelColor = RGBColor();

                for(int samples = 0; samples < sampler->mNumSamplesPerSet; ++samples)
                {
                    unitSample = sampler->getNextUnitSquareSample();
                    pixelSample.first = pixelSize * (c - 0.5 * renderTarget->getWidth() + unitSample.first);
                    pixelSample.second = pixelSize * (r - 0.5 * renderTarget->getHeight() + unitSample.second);
                    ray.origin = Point3d(pixelSample.first, pixelSample.second, zw);
                    pixelColor += rayTracer->traceRay(sceneManager.get(), ray);
                }

                pixelColor /= sampler->mNumSamplesPerSet;
                renderTarget->drawPixel(c, r, pixelColor);
            }
        }
    }

    virtual void renderScenePerspective()
    {
        RGBColor pixelColor;
        Ray ray;
        double zw = 100.0;
        double x, y;
        int samples = 16;
        int sampleSqrt = static_cast<int>(sqrt(samples));
        double viewPlaneDistance = 100.0;

        ray.origin = Vector3d(0.0, 0.0, zw);

        for(int r = 0; r < renderTarget->getHeight(); ++r)
        {
            for(int c = 0; c < renderTarget->getWidth(); ++c)
            {
                pixelColor = RGBColor();

                for(int vertical = 0; vertical < sampleSqrt; ++vertical)
                {
                    for(int horizontal = 0; horizontal < sampleSqrt; ++horizontal)
                    {
                        x = pixelSize * (c - 0.5 * renderTarget->getWidth() + (horizontal + 0.5) / sampleSqrt);
                        y = pixelSize * (r - 0.5 * renderTarget->getHeight() + (vertical + 0.5) / sampleSqrt);
                        ray.direction = Point3d(x, y, -viewPlaneDistance);
                        ray.direction.normalize();
                        pixelColor += rayTracer->traceRay(sceneManager.get(), ray);
                    }
                }

                pixelColor /= samples;
                renderTarget->drawPixel(c, r, pixelColor);
            }
        }
    }
};

}

#endif