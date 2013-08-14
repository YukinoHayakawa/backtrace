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

#ifndef BACKTRACE_SAMPLER_JITTERED_HPP
#define BACKTRACE_SAMPLER_JITTERED_HPP

#include "sampler.hpp"
#include <iostream>

namespace backtrace {

class JitteredSampler : public Sampler
{
public:
    virtual void generateSamples()
    {
        // TO-DO: Template debuger.
        // TO-DO: Reserve memory for vectors.
        std::cout << "JitteredSampler::generateSamples" << std::endl;

        int n = static_cast<int>(sqrt(mNumSamplesPerSet));

        for(int set = 0; set < mNumSampleSets; ++set)
        {
            for(int height = 0; height < n; ++height)
            {
                for(int width = 0; width < n; ++width)
                {
                    Point2d sample(
                        (static_cast<float>(width) + randomFloat()) / static_cast<float>(n),
                        (static_cast<float>(height) + randomFloat()) / static_cast<float>(n)
                    );
                    std::cout << "Sample generated: " << sample.first << " " << sample.second << std::endl;
                    mUnitSquaredSamples.push_back(sample);
                }
            }
        }
    }
};

}

#endif
