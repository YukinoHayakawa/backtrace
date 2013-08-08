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

#include "../detail/dispatcher.hpp"

using namespace std;
using namespace backtrace;

typedef NativeEvent<> SimpleEvent;

int main()
{
    Factory<EventTarget> factory;
    EventDispatcher dispatcher(4);

    auto root = factory.produce<EventTarget>();

    int i = 0;

    auto callback = [&i](Event* evt)
    {
        ++i;
    };

    root->addEventListener<SimpleEvent::Listener>("unknown", callback);
    root->addEventListener<SimpleEvent::Listener>("unknown", callback);
    root->addEventListener<SimpleEvent::Listener>("unknown", callback);
    root->addEventListener<SimpleEvent::Listener>("unknown", callback);
    root->addEventListener<SimpleEvent::Listener>("unknown", callback);

    for(auto t = 0; t < 1000000; ++t)
    {
        auto evt1 = dispatcher.createEvent<SimpleEvent>("unknown", root);

        evt1->addCallback(callback);
        evt1->addCallback(callback);
        evt1->addCallback(callback);
        evt1->addCallback(callback);
        evt1->addCallback(callback);

        dispatcher.postEvent(evt1);
    }

    cout << i << "\n";
}
