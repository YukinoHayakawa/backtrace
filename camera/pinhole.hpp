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

#ifndef BACKTRACE_CAMERA_PINHOLE_HPP
#define BACKTRACE_CAMERA_PINHOLE_HPP

#include "../scene/scene.hpp"
#include "../rendertarget/rendertarget.hpp"
#include "../raytracer/raytracer.hpp"
#include "../sampler/sampler.hpp"

#include "camera.hpp"

#include <iostream>
#include <iomanip>

namespace backtrace {

class PinholeCamera : public Camera
{
protected:
    float mViewDistance;
    float mZoom;

public:
    PinholeCamera(float viewDistance, float zoom)
        : mViewDistance(viewDistance),
        mZoom(zoom)
    {
    }

    virtual ~PinholeCamera() {}

    Vector3d rayDirection(const Point2d& p) const
    {
        Vector3d dir = p.x * mU + p.y * mV - mViewDistance * mW;
        dir.normalize();
        return dir;
    }

    virtual void renderScene(Scene* scene, RenderTarget* target, RayTracer* tracer)
    {
        RGBColor pixelColor;
        Ray ray;
        int recursionDepth = 0;
        Point2d samplePointUnit, samplePointPixel;

        float pixelSize = target->getPixelSize() / mZoom;
        ray.origin = mEyePosition;

        for(int r = 0; r < target->getHeight(); ++r)
        {
            for(int c = 0; c < target->getWidth(); ++c)
            {
                pixelColor = RGBColor();

                for(int numSample = 0; numSample < mSampler->getSampleNumber(); ++numSample)
                {
                    samplePointUnit = mSampler->getNextUnitSquareSample();
                    samplePointPixel.x = pixelSize * (c - 0.5 * target->getWidth() + samplePointUnit.x);
                    samplePointPixel.y = pixelSize * (r - 0.5 * target->getHeight() + samplePointUnit.y);
                    ray.direction = rayDirection(samplePointPixel);
                    pixelColor += tracer->traceRay(&ray, scene);
                }
                pixelColor /= mSampler->getSampleNumber();
                pixelColor *= mExposureTime;
                target->drawPixel(c, r, pixelColor);
            }
        }
    }
};

}

#endif
