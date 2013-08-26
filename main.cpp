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

#include <memory>

#include "engine.hpp"
#include "scene/scene.hpp"
#include "rendertarget/window/sdlwindow/sdlwindow.hpp"
#include "rendertarget/window/sdlwindow/sdlwindow.cpp"
#include "object/primitive/sphere.hpp"
#include "object/primitive/plane.hpp"
#include "raytracer/simple.hpp"
#include "sampler/jittered.hpp"
#include "camera/pinhole.hpp"

using namespace backtrace;

int main()
{
    Engine engine;

    engine.scene.reset(new Scene());
    engine.renderTarget.reset(new SDLWindow(64, 48, 32, 0.1, 1.0, false));
    engine.rayTracer.reset(new SimpleRayTracer());
    engine.camera.reset(new PinholeCamera(10.0, 1.0));
    engine.camera->setEyePosition(Vector3d(0.0, 0.0, 10.0));
    engine.camera->computeUVW();
    engine.camera->setSimpler(new JitteredSampler());

    auto sphere = engine.scene->addObject<Sphere>();
    sphere->setColor(RGBColor(1, 0, 0));
    sphere->mCenter = (0, 0.5, 0);

    sphere = engine.scene->addObject<Sphere>();
    sphere->setColor(RGBColor(1, 1, 0));
    sphere->mCenter = (0, 0, 0.5);

    engine.renderScene();

    engine.inputDevicesTarget->addEventListener<KeyEvent::Listener>(
        Engine::buildKeyEventName(SDLK_ESCAPE, Engine::Pressed),
        [&engine](Event*) { engine.shouldExit(); }
    );

    engine.waitExit();
}
