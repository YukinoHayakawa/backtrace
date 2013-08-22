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
#include <mutex>
#include <condition_variable>

#include "SDL2/SDL.h"

#include "event/event.hpp"
#include "object/geometricobject.hpp"
#include "math/math.hpp"
#include "raytracer/raytracer.hpp"
#include "rendertarget/rendertarget.hpp"
#include "scenemanager/scenemanager.hpp"
#include "sampler/sampler.hpp"
#include "util/workqueue.hpp"
#include "gameevents.hpp"

namespace backtrace {

class Engine
{
public:
    std::unique_ptr<SceneManager> sceneManager;
    std::unique_ptr<RenderTarget> renderTarget;
    std::unique_ptr<RayTracer> rayTracer;
    std::unique_ptr<Sampler> sampler;
    RGBColor backgroundColor;

    Factory<EventTarget> eventTargetFactory;
    EventDispatcher eventDispatcher;

    WorkQueue defaultWorkQueue;

    EventTarget* rootTarget;
    EventTarget* inputDevicesTarget;
    EventTarget* rendererTarget;
    // EventTarget* physicsTarget;
    // EventTarget* guiTarget;

    bool shouldContinue;
    bool pendingExit;

    std::mutex waitExitMutex;
    std::condition_variable waitExitCV;

public:
    Engine(SceneManager* sceneManager,
        RenderTarget* renderTarget,
        RayTracer* rayTracer,
        Sampler* sampler)
        : sceneManager(sceneManager),
        renderTarget(renderTarget),
        rayTracer(rayTracer),
        sampler(sampler),
        eventDispatcher(4),
        defaultWorkQueue(4),
        shouldContinue(true),
        pendingExit(false)
    {
        sampler->generateSamples();

        rootTarget = eventTargetFactory.produce<EventTarget>();
        inputDevicesTarget = eventTargetFactory.produce<EventTarget>();
        rendererTarget = eventTargetFactory.produce<EventTarget>();
        
        inputDevicesTarget->setParent(rootTarget);
        rendererTarget->setParent(rootTarget);

        defaultWorkQueue.push(std::bind(&Engine::runEventLoop, this));
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
                    pixelSample.x = renderTarget->getPixelSize() * (c - 0.5 * renderTarget->getWidth() + unitSample.x);
                    pixelSample.y = renderTarget->getPixelSize() * (r - 0.5 * renderTarget->getHeight() + unitSample.y);
                    ray.origin = Point3d(pixelSample.x, pixelSample.y, zw);
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
                        x = renderTarget->getPixelSize() * (c - 0.5 * renderTarget->getWidth() + (horizontal + 0.5) / sampleSqrt);
                        y = renderTarget->getPixelSize() * (r - 0.5 * renderTarget->getHeight() + (vertical + 0.5) / sampleSqrt);
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

    void runEventLoop()
    {
        SDL_Event event;

        while(shouldContinue && SDL_WaitEvent(&event))
        {
            switch(event.type)
            {
            case SDL_KEYDOWN:
                eventDispatcher.postEvent<KeyEvent>(
                    buildKeyEventName(event.key.keysym.sym, Pressed), inputDevicesTarget
                );
                break;

            case SDL_KEYUP:
                eventDispatcher.postEvent<KeyEvent>(
                    buildKeyEventName(event.key.keysym.sym, Released), inputDevicesTarget
                );
                break;

            case SDL_MOUSEMOTION:
                eventDispatcher.postEvent<MouseMotionEvent>(
                    "onMouseMoved", inputDevicesTarget,
                    event.motion.x, event.motion.y,
                    event.motion.xrel, event.motion.yrel
                );
                break;

            case SDL_MOUSEBUTTONDOWN:
                eventDispatcher.postEvent<MouseKeyEvent>(
                    buildMouseKeyEventName(event.button.button, Pressed), inputDevicesTarget,
                    event.button.x, event.button.y
                );
                break;

            case SDL_MOUSEBUTTONUP:
                eventDispatcher.postEvent<MouseKeyEvent>(
                    buildMouseKeyEventName(event.button.button, Released), inputDevicesTarget,
                    event.button.x, event.button.y
                );
                break;

            default:
                break;
            }
        }
    }

    bool shouldExit()
    {
        if(pendingExit) return false;
        pendingExit = true;

        auto evt = eventDispatcher.createEvent<ExitRequestedEvent>("onExitRequested", rootTarget);
        evt->addCallback([this](Event*) { shouldContinue = false; waitExitCV.notify_all(); });
        eventDispatcher.postEvent(evt);

        return true;
    }

    enum KeyState
    {
        Pressed = true,
        Released = false
    };

    static std::string buildKeyEventName(SDL_Keycode keyCode, KeyState keyState)
    {
        std::string dest("onKey");
        dest += (keyCode + 1);
        dest += keyState ? "Pressed" : "Released";
        return dest;
    }

    static std::string buildMouseKeyEventName(SDL_Keycode keyCode, KeyState keyState)
    {
        std::string dest("onMouseKey");
        dest += (keyCode + 1);
        dest += keyState ? "Pressed" : "Released";
        return dest;
    }

    void waitExit()
    {
        std::unique_lock<std::mutex> lock(waitExitMutex);
        waitExitCV.wait(lock, [this]{ return pendingExit; });
    }
};

}

#endif
