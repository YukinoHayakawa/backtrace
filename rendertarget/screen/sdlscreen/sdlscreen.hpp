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

#ifndef BACKTRACE_RENDERTARGET_SDLSCREEN_HPP
#define BACKTRACE_RENDERTARGET_SDLSCREEN_HPP

#include <memory>
#include <thread>
#include <mutex>

#include "../screen.hpp"

struct SDL_Surface;

namespace backtrace {

class SDLScreen : public Screen
{
protected:
    SDL_Surface* mScreen;
    bool mFullscreen;

public:
    SDLScreen(uint32_t width, uint32_t height, uint8_t bitDepth, bool fullscreen);
    virtual ~SDLScreen();

    virtual void drawPixel(uint32_t x, uint32_t y, uint32_t color);
    virtual void drawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    virtual void drawPixel(uint32_t x, uint32_t y, const RGBColor& color);
    
    virtual void update(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};

}

#endif
