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

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "sdlscreen.hpp"

namespace backtrace {

    SDLScreen::SDLScreen(uint32_t width, uint32_t height, uint8_t bitDepth, bool fullscreen)
        : Screen(width, height, bitDepth),
        mFullscreen(fullscreen)
    {
        // Initialize SDL's subsystems - in this case, only video.
        if(SDL_Init(SDL_INIT_VIDEO) < 0) 
        {
            //throw std::runtime_error("Unable to init SDL: " + SDL_GetError());
            fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
            throw 0;
        }

        // Attempt to create a window with pixels under specific mode.
        mScreen = SDL_SetVideoMode(width, height, bitDepth,
            fullscreen ? (SDL_HWSURFACE | SDL_FULLSCREEN) : SDL_HWSURFACE);

        // If we fail, return error.
        if(mScreen == NULL) 
        {
            //throw std::runtime_error()
            fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
            exit(1);
        }
    }

    SDLScreen::~SDLScreen()
    {
        // Register SDL_Quit to be called at exit; makes sure things are
        // cleaned up when we quit.
        SDL_Quit();
    }

    void SDLScreen::drawPixel(uint32_t x, uint32_t y, uint32_t color)
    {
        unsigned int *ptr = (unsigned int*)mScreen->pixels;
        int lineoffset = y * (mScreen->pitch / 4);
        ptr[lineoffset + x] = color;
    }

    void SDLScreen::drawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        drawPixel(x, y, SDL_MapRGB(mScreen->format, r, g, b));
    }

    void SDLScreen::update(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        // Tell SDL to update the screen
        SDL_UpdateRect(mScreen, x, y, width, height);
    }

}
