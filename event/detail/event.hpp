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

#ifndef BACKTRACE_EVENT_DETAIL_EVENT_H
#define BACKTRACE_EVENT_DETAIL_EVENT_H

#include <vector>
#include <string>
#include <functional>
#include <exception>
 
namespace backtrace {

class EventTarget;

class EventListener
{
public:
    virtual ~EventListener() {}

    virtual void handleEvent(class Event* event) {};
};

class Event
{
public:
    std::string type;
    EventTarget* target;
    EventTarget* currentTarget;
    bool cancelled;

    Event(std::string type, EventTarget* target)
        : type(std::move(type)),
        target(target),
        currentTarget(nullptr),
        cancelled(false)
    {
    }

    virtual ~Event()
    {
        auto end = m_callbacks.end();
        for(auto i = m_callbacks.begin(); i != end; ++i)
        {
            (*i)(this);
        }
    }

    // Will be called when event destruct.
    void addCallback(std::function<void(Event*)> callback)
    {
        m_callbacks.push_back(std::move(callback));
    }

    void cancel()
    {
        cancelled = true;
    }

protected:
    std::vector<std::function<void(Event*)>> m_callbacks;
};

template<typename...Args>
class SpecifiedEvent : public Event
{
    friend class EventListener;
    typedef std::function<void(Event*, Args...)> HandlerType;

public:
    SpecifiedEvent(std::string type, EventTarget* target,
        Args...args)
        : Event(std::move(type), target),
        m_invoker(std::bind(&SpecifiedEvent::_invoke, this,
            std::placeholders::_1, args...))
    {
    }

    virtual ~SpecifiedEvent() {}

    class Listener : public EventListener
    {
    public:
        Listener(HandlerType handler)
            : m_handler(std::move(handler))
        {
        }

        virtual ~Listener() {}

        virtual void handleEvent(Event* event)
        {
            typedef SpecifiedEvent<Args...> RealEvent;
            // Just for performance reason that I don't use dynamic_cast.
            // Take care of yourself.
            RealEvent* p = static_cast<RealEvent*>(event);
            p->m_invoker(m_handler);
        }

    protected:
        HandlerType m_handler;
    };

protected:
    // TO-DO: Reduce this indirectional callling layer.
    void _invoke(const HandlerType& handler, Args...args)
    {
        // Call event handler. ##Callback(Event*, Args...);
        handler(this, args...);
    }

    std::function<void(const HandlerType&)> m_invoker;
};

} // namespace backtrace

#endif
