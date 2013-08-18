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

#include <cmath>

#include "SDL2/SDL.h"

#include "sdlwindow.hpp"

namespace backtrace {

SDLWindow::SDLWindow(uint32_t width, uint32_t height, uint8_t colorDepth, float pixelSize, float gamma, bool fullscreen)
    : Window(width, height, colorDepth, pixelSize, gamma),
    mFullscreen(fullscreen)
{
    // Initialize SDL's subsystems - in this case, only video.
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) 
    {
        //throw std::runtime_error("Unable to init SDL: " + SDL_GetError());
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        throw 0;
    }

    // Attempt to create a window with pixels under specific mode.
    SDL_CreateWindowAndRenderer(width, height, 0, &mWindow, &mRenderer);

    // If we fail, return error.
    if(mWindow == nullptr || mRenderer == nullptr) 
    {
        //throw std::runtime_error()
        fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    mDrawCanvas = SDL_CreateTexture(
        mRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

    mPixels.reset(new uint32_t[width * height]);
}

SDLWindow::~SDLWindow()
{
    // Register SDL_Quit to be called at exit; makes sure things are
    // cleaned up when we quit.
    SDL_Quit();
}

void SDLWindow::drawPixel(uint32_t x, uint32_t y, uint32_t color)
{
    int pitch = mWidth * sizeof(uint32_t);
    int lineoffset = y * (pitch / 4);
    mPixels[lineoffset + x] = color;
}

void SDLWindow::drawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = 0;
    color += r;
    color = color << 8;
    color += g;
    color = color << 8;
    color += b;
    drawPixel(x, y, color);
}

void SDLWindow::drawPixel(uint32_t x, uint32_t y, const RGBColor& color)
{
    if(mGamma == 1.0)
    {
        drawPixel(x, y, color.r * 255, color.g * 255, color.b * 255);
    }
    else
    {
        drawPixel(x, y,
            pow(color.r, mInvGamma) * 255,
            pow(color.g, mInvGamma) * 255,
            pow(color.b, mInvGamma) * 255
        );
    }
}

void SDLWindow::update(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    SDL_UpdateTexture(mDrawCanvas, NULL, mPixels.get(), mWidth * sizeof(uint32_t));
    SDL_RenderCopy(mRenderer, mDrawCanvas, NULL, NULL);
    SDL_RenderPresent(mRenderer);
}

}
