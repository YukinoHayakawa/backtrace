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

#include "../sdlscreen.cpp"

using namespace backtrace;

int main()
{
    auto canvas = SDLScreen(640, 480, 32, false);

    for(auto i = 0; i < 480; ++i)
    {
        for(auto j = 0; j < 640; ++j)
        {
            canvas.drawPixel(j, i, 255, 0, 0);
        }
    }

    canvas.updateWholeScreen();

    std::chrono::milliseconds dura( 2000 );
    std::this_thread::sleep_for( dura );
}
