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

#include <iostream>

#include "../workqueue.hpp"

using namespace std;
using namespace backtrace;

void callback(int i)
{
    cout << i << "\n";
}

int main()
{
    WorkQueue workers(400);

    for(auto i = 0; i < 1000000; ++i)
    {
        workers.push(std::bind(&callback, i));
    }
}
