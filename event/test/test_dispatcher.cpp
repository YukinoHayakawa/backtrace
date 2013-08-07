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
#include <unordered_map>
#include <string>

#include "../detail/dispatcher.hpp"

using namespace std;
using namespace backtrace;

typedef SpecifiedEvent<std::string> SimpleEvent;

int main()
{
    std::unordered_map<EventTarget*, std::string> mapping;
    Factory<EventTarget> factory;
    EventDispatcher dispatcher(1);

    cout << "Building structure..." << "\n";

    auto root = factory.produce<EventTarget>();
    auto node1 = factory.produce<EventTarget>();
    node1->setParent(root);
    auto node2 = factory.produce<EventTarget>();
    node2->setParent(node1);
    auto node3 = factory.produce<EventTarget>();
    node3->setParent(node1);
    auto node4 = factory.produce<EventTarget>();
    node4->setParent(node1);
    auto node5 = factory.produce<EventTarget>();
    node5->setParent(node1);
    auto node6 = factory.produce<EventTarget>();
    node6->setParent(node1);

    mapping.insert(std::make_pair(root, "root"));
    mapping.insert(std::make_pair(node1, "node1"));
    mapping.insert(std::make_pair(node2, "node2"));
    mapping.insert(std::make_pair(node3, "node3"));
    mapping.insert(std::make_pair(node4, "node4"));
    mapping.insert(std::make_pair(node5, "node5"));
    mapping.insert(std::make_pair(node6, "node6"));

    /*
     *  Current Structure:
     * 
     *         Root
     *          /
     *        Node1
     *       /    \
     *   Node2 ... Node6
     *
     */

    auto callback = [&mapping](Event* evt, std::string name)
    {
        cout << name << "\n";
        cout << "type " << evt->type << "\n";
        cout << "target " << evt->target << "\n";
        cout << "currentTarget " << mapping[evt->currentTarget] << "\n";
        cout << "currentTargetPtr " << evt->currentTarget << "\n";
        cout << "cancelled " << evt->cancelled << "\n";
        cout << "\n";
    };

    auto callback2 = [](Event* evt, std::string)
    {
        evt->cancel();
    };

    root->addEventListener<SimpleEvent::Listener>("unknown", callback);
    node1->addEventListener<SimpleEvent::Listener>("unknown", callback);
    node1->addEventListener<SimpleEvent::Listener>("unknown", [node1](Event*, std::string) {
        node1->iterateChildren([](EventTarget* t) { cout << "child " << t << "\n"; });
    });
    node2->addEventListener<SimpleEvent::Listener>("unknown", callback);
    node3->addEventListener<SimpleEvent::Listener>("unknown", callback);

    node4->addEventListener<SimpleEvent::Listener>("unknown", callback);
    auto id = node4->addEventListener<SimpleEvent::Listener>("unknown", callback);
    node4->removeEventListener(id);

    node5->addEventListener<SimpleEvent::Listener>("unknown", callback);

    node6->addEventListener<SimpleEvent::Listener>("unknown", callback);

    auto evt1 = dispatcher.createEvent<SimpleEvent>("unknown", node1, "evt1");

    evt1->addCallback([&mapping](Event*) {
        for(auto i = mapping.begin(); i != mapping.end(); ++i)
            cout << i->first << ": " << i->second << " parent: " << mapping[i->first->getParent()] <<"\n";
        cout << "event1 finished dispatching.\n";
    });

    dispatcher.postEvent(evt1);

    std::chrono::milliseconds dura(200);
    std::this_thread::sleep_for(dura);

    node5->addEventListener<SimpleEvent::Listener>("unknown", callback2);

    dispatcher.postEvent<SimpleEvent>("unknown", root, "evt2");
}
