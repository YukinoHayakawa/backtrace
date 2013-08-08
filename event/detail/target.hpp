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

#ifndef BACKTRACE_EVENT_DETAIL_TARGET_H
#define BACKTRACE_EVENT_DETAIL_TARGET_H

#include <unordered_map>
#include <string>
#include <utility>
#include <mutex>
#include <atomic>

#include "event.hpp"
#include "../../util/factory.hpp"

namespace backtrace {

class EventTarget
{
    typedef std::unordered_multimap<std::string, EventListener*> HandlerMap;
    typedef void* ListenerId;

protected:
    HandlerMap m_handlers;
    std::mutex m_handlerMutex;
    std::mutex m_childrenMutex;
    // TO-DO: Listener order.
    Factory<EventListener> m_listenerFactory;

    std::atomic<EventTarget*> m_parent;
    std::unordered_set<EventTarget*> m_children;

public:
    EventTarget()
        : m_parent(nullptr)
    {
    }

    virtual ~EventTarget() {}

    template<typename T, typename...Args>
    ListenerId addEventListener(const std::string& type, Args...args)
    {
        auto naked = m_listenerFactory.produce<T>(std::forward<Args>(args)...);

        std::lock_guard<std::mutex> lock(m_handlerMutex);
        m_handlers.insert(std::make_pair(type, naked));
        return static_cast<ListenerId>(naked);
    }

    bool removeEventListener(ListenerId listener)
    {
        std::lock_guard<std::mutex> lock(m_handlerMutex);
        auto end = m_handlers.end();
        for(auto i = m_handlers.begin(); i != end; ++i)
        {
            if(i->second == listener)
            {
                m_handlers.erase(i);
                m_listenerFactory.recycle(static_cast<EventListener*>(listener));
                return true;
            }
        }
        return false;
    }

    void dispatchEvent(Event* evt)
    {
        _dispatchEvent(evt, true, true);
    }

    EventTarget* getParent()
    {
        return m_parent;
    }

    void setParent(EventTarget* parent)
    {
        if(m_parent)
        {
            m_parent.load()->_removeChild(this);
        }

        parent->_addChild(this);
        m_parent = parent;
    }

    void iterateChildren(const std::function<void(EventTarget*)>& function)
    {
        std::lock_guard<std::mutex> lock(m_childrenMutex);

        auto end = m_children.end();
        for(auto i = m_children.begin(); i != end; ++i)
        {
            function(*i);
        }
    }

protected:
    void _removeChild(EventTarget* child)
    {
        std::lock_guard<std::mutex> lock(m_childrenMutex);
        m_children.erase(child);
    }

    void _addChild(EventTarget* child)
    {
        std::lock_guard<std::mutex> lock(m_childrenMutex);
        m_children.insert(child);
    }

    void _dispatchEvent(Event* evt, bool passToParent, bool passToChildren)
    {
        evt->currentTarget = this;
        
        {
            std::lock_guard<std::mutex> lock(m_handlerMutex);

            auto handlers = m_handlers.equal_range(evt->type);
            for (auto i = handlers.first; i != handlers.second; ++i) {
                if(evt->cancelled)
                {
                    return;
                }
                
                m_listenerFactory.protect(i->second);
                i->second->handleEvent(evt);
                m_listenerFactory.unprotect(i->second);
            }
        }

        if(passToParent && m_parent)
        {
            m_parent.load()->_dispatchEvent(evt, true, false);
        }

        if(passToChildren && m_children.size())
        {
            std::lock_guard<std::mutex> lock(m_childrenMutex);

            auto end = m_children.end();
            for(auto i = m_children.begin(); i != end; ++i)
            {
                (*i)->_dispatchEvent(evt, false, true);
            }
        }
    }
};

} // namespace backtrace

#endif
