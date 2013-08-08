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
#include <tuple>

namespace backtrace {

// N is the amount of params.
template<size_t N>
struct TupleExpanderForNative
{
    template<typename Functor, typename Tuple, typename...Args>
    static void expandTuple(Functor& functor, Tuple& tuple, Args...expanded)
    {
        TupleExpanderForNative<N - 1>::template expandTuple<Functor, Tuple>(
            functor, tuple, std::get<N - 1>(tuple), std::forward<Args>(expanded)...
            // N - 1 because of index's shifting.
        );
    }
};

template<>
struct TupleExpanderForNative<0>
{
    template<typename Functor, typename Tuple, typename...Args>
    static void expandTuple(Functor& functor, Tuple& tuple, Args...expanded)
    {
        functor(std::forward<Args>(expanded)...);
    }
};

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

template<typename TupleExpander, typename...Args>
class SpecifiedEvent : public Event
{
    typedef std::function<void(Event*, Args...)> HandlerType;
    typedef std::tuple<Event*, Args...> ArgPack;

protected:
    ArgPack mArgPack;

public:
    SpecifiedEvent(std::string type, EventTarget* target, Args...args)
        : Event(std::move(type), target),
        mArgPack(this, std::move(args)...)
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

        virtual void handleEvent(Event* evt)
        {
            auto p = static_cast<SpecifiedEvent*>(evt);
            TupleExpander::template expandTuple<HandlerType, ArgPack>(
                m_handler, p->mArgPack
            );
        }

    protected:
        HandlerType m_handler;
    };
};

template<typename...Args>
class NativeEvent : public SpecifiedEvent<TupleExpanderForNative<sizeof...(Args) + 1>, Args...>
{ // The "+ 1" above is for the "Event*" param.
     typedef SpecifiedEvent<TupleExpanderForNative<sizeof...(Args) + 1>, Args...> MyBase;
public:
    NativeEvent(std::string type, EventTarget* target, Args...args)
        : MyBase(std::move(type), target, std::forward<Args>(args)...)
    {
    }
};

// TO-DO: Event interface for script programming.

} // namespace backtrace

#endif
