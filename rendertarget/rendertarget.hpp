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

#ifndef BACKTRACE_RENDERTARGETBASE_HPP
#define BACKTRACE_RENDERTARGETBASE_HPP

#include "../util/rgbcolor.hpp"

namespace backtrace {

class RenderTarget
{
protected:
    uint32_t mWidth, mHeight;
    uint8_t mBitDepth;
    float mAspectRatio;
    float mGamma;
    float mInvGamma;

public:
    RenderTarget(uint32_t width, uint32_t height, uint8_t bitDepth)
        : mWidth(width),
        mHeight(height),
        mBitDepth(bitDepth),
        mAspectRatio(static_cast<float>(mWidth) / static_cast<float>(mHeight)),
        mGamma(1.0),
        mInvGamma(1.0)
    {
    }

    virtual ~RenderTarget() {};

    virtual void drawPixel(uint32_t x, uint32_t y, uint32_t color) = 0;
    virtual void drawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void drawPixel(uint32_t x, uint32_t y, const RGBColor& color) = 0;
    
    void setGamma(float gamma)
    {
        mGamma = gamma;
        mInvGamma = 1.0f / gamma;
    }

    float getGamma() { return mGamma; }
    float getInvGamma() { return mInvGamma; }
    uint32_t getWidth() { return mWidth; }
    uint32_t getHeight() { return mHeight; }
    uint8_t getBitDepth() { return mBitDepth; }
    float getAspectRatio() { return mAspectRatio; }

    virtual void setResolution(uint32_t width, uint32_t height, uint8_t bitDepth)
    {
        mWidth = width;
        mHeight = height;
        mBitDepth = bitDepth;
        mAspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
    }

    virtual void update(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}

    void update()
    {
        update(0, 0, mWidth, mHeight);
    }
};

}

#endif
