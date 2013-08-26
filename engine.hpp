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
#include "scene/scene.hpp"
#include "sampler/sampler.hpp"
#include "util/workqueue.hpp"
#include "gameevents.hpp"
#include "camera/camera.hpp"
 
namespace backtrace {

class Engine
{
public:
    std::unique_ptr<Scene> scene;
    std::unique_ptr<RenderTarget> renderTarget;
    std::unique_ptr<RayTracer> rayTracer;
    std::unique_ptr<Camera> camera;
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
    Engine()
        : eventDispatcher(4),
        defaultWorkQueue(4),
        shouldContinue(true),
        pendingExit(false)
    {
        rootTarget = eventTargetFactory.produce<EventTarget>();
        inputDevicesTarget = eventTargetFactory.produce<EventTarget>();
        rendererTarget = eventTargetFactory.produce<EventTarget>();
        
        inputDevicesTarget->setParent(rootTarget);
        rendererTarget->setParent(rootTarget);

        defaultWorkQueue.push(std::bind(&Engine::runEventLoop, this));
    }

    virtual ~Engine() {}

    void renderScene()
    {
        camera->renderScene(scene.get(), renderTarget.get(), rayTracer.get());
        renderTarget->update();
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
