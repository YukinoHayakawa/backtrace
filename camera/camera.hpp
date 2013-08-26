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

#ifndef BACKTRACE_CAMERA_BASE_HPP
#define BACKTRACE_CAMERA_BASE_HPP

#include <memory>

#include "../math/math.hpp"

namespace backtrace {

class Camera
{
public:
    Camera()
        : mExposureTime(1.0),
        mUp(0.0, 1.0, 0.0)
    {
    }

    virtual ~Camera() {}

    void computeUVW()
    {
        mW = mEyePosition - mLookAt;
        mW.normalize();
        mU = mUp.cross(mW);
        mU.normalize();
        mV = mW.cross(mU);
    }

    virtual void renderScene(class Scene* scene, class RenderTarget* target, class RayTracer* tracer) {}

    Point3d getEyePosition() { return mEyePosition; }
    void setEyePosition(const Point3d& pos) { mEyePosition = pos; }

    Point3d getLookAt() { return mLookAt; }
    void setLookAt(const Point3d& at) { mLookAt = at; }

    Vector3d getUp() { return mUp; }

    float getExposureTime() { return mExposureTime; }
    void setExposureTime(float time) { mExposureTime = time; }

    void setSimpler(class Sampler* sampler)
    {
        mSampler.reset(sampler);
        mSampler->generateSamples();
    }

protected:
    Point3d mEyePosition;
    Point3d mLookAt;
    Vector3d mUp;
    Vector3d mU, mV, mW;
    float mExposureTime;
    std::unique_ptr<class Sampler> mSampler;
};

}

#endif
