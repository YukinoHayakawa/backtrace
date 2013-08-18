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

#ifndef BACKTRACE_RENDERTARGET_SDLWINDOW_HPP
#define BACKTRACE_RENDERTARGET_SDLWINDOW_HPP

#include <memory>
#include <thread>
#include <mutex>

#include "../window.hpp"

namespace backtrace {

class SDLWindow : public Window
{
protected:
    struct SDL_Window* mWindow;
    struct SDL_Renderer* mRenderer;
    struct SDL_Texture* mDrawCanvas;
    std::unique_ptr<uint32_t[]> mPixels;

    bool mFullscreen;

public:
    SDLWindow(uint32_t width, uint32_t height, uint8_t colorDepth, float pixelSize, float gamma, bool fullscreen);
    virtual ~SDLWindow();

    virtual void drawPixel(uint32_t x, uint32_t y, uint32_t color);
    virtual void drawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    virtual void drawPixel(uint32_t x, uint32_t y, const RGBColor& color);
    
    virtual void update(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};

}

#endif
