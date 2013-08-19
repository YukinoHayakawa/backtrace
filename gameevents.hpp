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

#ifndef BACKTRACE_GAME_EVENT_DEF_HPP
#define BACKTRACE_GAME_EVENT_DEF_HPP

#include "event/event.hpp"

namespace backtrace {

//-------------------------------------------------- Input Events

// onKey***Pressed, onKey***Released <>
typedef NativeEvent<> KeyEvent;
// onMouseMoved <XAbs, YAbs, XRel, YRel>
typedef NativeEvent<uint32_t, uint32_t, uint32_t, uint32_t> MouseMotionEvent;
// onMouseKey***Pressed, onMouseKey***Released <XAbs, YAbs>
typedef NativeEvent<uint32_t, uint32_t> MouseKeyEvent;

//-------------------------------------------------- Application Events

// onExitRequedted <>
typedef NativeEvent<> ExitRequestedEvent;

//-------------------------------------------------- Renderer Events

// onRectRerenderRequested <X, Y, width, height>
typedef NativeEvent<uint32_t, uint32_t, uint32_t, uint32_t> RectRerenderRequested;
// onFullRerenderRequested <>
typedef NativeEvent<> FullRerenderRequested;

}

#endif
