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

#ifndef BACKTRACE_FACTORY_H
#define BACKTRACE_FACTORY_H

#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <memory>

namespace backtrace {

// TO-DO: customizeable memory allocation
template<typename T>
class Factory
{
protected:
    std::unordered_set<T*> m_protected;
    std::unordered_set<T*> m_pendingDelete;
    std::unordered_map<T*, std::unique_ptr<T>> m_products;
    std::mutex m_mutex;

    bool _forceRecycle(T* product)
    {
        std::unique_ptr<T> p;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            m_protected.erase(product);
            m_pendingDelete.erase(product);

            auto i = m_products.find(product);
            if(i == m_products.end())
            {
                return false;
            }

            p.reset(i->second.release());
            m_products.erase(product);
        }
        
        return true;
    }

public:
    virtual ~Factory() {}

    template<typename P, typename...Args>
    P* produce(Args...args)
    {
        P* naked = new P(std::forward<Args>(args)...);
        std::unique_ptr<T> pack(naked);

        std::lock_guard<std::mutex> lock(m_mutex);

        m_products.insert(std::make_pair(naked, std::move(pack)));
        return naked;
    }

    // Destroy product.
    // Return true if product has been destroyed. Otherwise
    // return false.
    // If product is protected, the deletion operation will
    // delay until it is unprotected again.
    bool recycle(T* product)
    {
        if(isProtected(product))
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_pendingDelete.insert(product);
            return false;
        }
        else
        {
            return _forceRecycle(product);
        }
    }

    // Prevent product from destroying
    bool protect(T* product)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_products.count(product))
        {
            return false;
        }
        return m_protected.insert(product).second;
    }

    bool unprotect(T* product)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_protected.erase(product))
        {
            return false;
        }
        else
        {
            auto pendingDetele = m_pendingDelete.find(product);
            if(pendingDetele != m_pendingDelete.end())
            {
                m_pendingDelete.erase(pendingDetele);
                return m_products.erase(product);
            }
        }
        return false;
    }

    bool isProtected(T* product)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_protected.count(product) == 1;
    }

    bool isPendingDelete(T* product)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_pendingDelete.count(product) == 1;
    }

    bool isAlive(T* product)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_products.count(product) == 1;
    }

    void iterateProducts(const std::function<void(const T&)>& function)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto end = m_products.end();
        for(auto iter = m_products.begin(); iter != end; ++iter)
        {
            function(*(iter->first));
        }
    }
};

} // namespace backtrace

#endif
