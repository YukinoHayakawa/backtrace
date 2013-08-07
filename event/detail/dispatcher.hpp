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

#ifndef BACKTRACE_EVENT_DETAIL_DISPATCHER_H
#define BACKTRACE_EVENT_DETAIL_DISPATCHER_H

#include <memory>
#include <thread>
#include <vector>

#include "event.hpp"
#include "target.hpp"
#include "../../util/factory.hpp"
#include "../../util/workqueue.hpp"

namespace backtrace {

class EventDispatcher
{
public:
    EventDispatcher(size_t workers)
        // TO-DO: m_pendingEvents(getCPUCores())
        : m_pendingEvents(workers)
    {
    }

    virtual ~EventDispatcher() {}

    template<typename T, typename...Args>
    void postEvent(Args...args)
    {
        postEvent(createEvent<T>(std::move(args)...));
    }

    void postEvent(Event* evt)
    {
        auto task = [evt, this]()
        {
            evt->target->dispatchEvent(evt);
            m_evtFactory.recycle(evt);
        };

        m_pendingEvents.push(task);
    }

    template<typename T, typename...Args>
    Event* createEvent(Args...args)
    {
        return m_evtFactory.produce<T>(std::move(args)...);
    }

protected:
    Factory<Event> m_evtFactory;
    WorkQueue m_pendingEvents;
};

} // namespace backtrace

#endif
