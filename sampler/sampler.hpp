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

#ifndef BACKTRACE_SAMPLERBASE_HPP
#define BACKTRACE_SAMPLERBASE_HPP

#include <vector>

#include "../math/math.hpp"

namespace backtrace {

class Sampler
{
public: // TO-DO: Should be protected.
    int mNumSamplesPerSet;
    int mNumSampleSets;
    std::vector<Point2d> mUnitSquaredSamples;
    std::vector<Point2d> mUnitDiskSamples;
    std::vector<Point3d> mHemisphereSamples;
    std::vector<int> mShuffledIndices;
    int mRandomIndexJump;
    int mIndexCount;

public:
    Sampler()
        : mNumSamplesPerSet(12),
        mNumSampleSets(1),
        mRandomIndexJump(0),
        mIndexCount(0)
    {
    }

    virtual void generateSamples() {}

    void setupShuffledIndices()
    {

    }

    void shuffleSamples()
    {

    }

    Point2d getNextUnitSquareSample()
    {
        // TO-DO: Random simple. Referrence: p81.
        return mUnitSquaredSamples[mIndexCount++ % (mNumSamplesPerSet * mNumSampleSets)];
    }

    void mapSamplesToUnitDisk()
    {
        float radius, phi;
        Point2d samplePoint;

        mUnitDiskSamples.reserve(mUnitSquaredSamples.size());

        auto end = mUnitSquaredSamples.end();
        for(auto iter = mUnitSquaredSamples.begin(); iter != end; ++iter)
        {
            samplePoint.x = 2.0 * (*iter).x - 1.0;
            samplePoint.y = 2.0 * (*iter).y - 1.0;

            if(samplePoint.x > -samplePoint.y)
            {
                if(samplePoint.x > samplePoint.y)
                {
                    radius = samplePoint.x;
                    phi = samplePoint.y / samplePoint.x;
                }
                else
                {
                    radius = samplePoint.y;
                    phi = 2 - samplePoint.x / samplePoint.y;
                }
            }
            else
            {
                if(samplePoint.x < samplePoint.y)
                {
                    radius = -samplePoint.x;
                    phi = 4 + samplePoint.y / samplePoint.x;
                }
                else
                {
                    radius = -samplePoint.y;
                    if(samplePoint.y != 0.0)
                    {
                        phi = 6 - samplePoint.x / samplePoint.y;
                    }
                    else
                    {
                        phi = 0.0;
                    }
                }
            }

            phi *= PI / 4.0;
            mUnitDiskSamples.push_back(Point2d(radius * cos(phi), radius * sin(phi)));
        }
    }

    void mapSamplesToHemisphere(float e)
    {
        mHemisphereSamples.reserve(mUnitSquaredSamples.size());

        float cosPhi, sinPhi, cosTheta, sinTheta;
        float temp;

        auto end = mUnitSquaredSamples.end();
        for(auto iter = mUnitSquaredSamples.begin(); iter != end; ++iter)
        {
            temp = 2.0 * PI * (*iter).x;
            cosPhi = cos(temp);
            sinPhi = sin(temp);
            cosTheta = pow(1.0 - (*iter).y, 1.0 / (e + 1.0));
            sinTheta = sqrt(1.0 - cosTheta * cosTheta);
            mHemisphereSamples.push_back(Point3d(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta));
        }
    }
};

}

#endif
