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

#ifndef BACKTRACE_TREE_H
#define BACKTRACE_TREE_H

#include <exception>
#include <unordered_set>
#include <mutex>

#include "factory.hpp"

namespace backtrace {

namespace exception {

class BadCast : public std::exception
{
};

} // namespace exception

template<typename T>
class Tree
{
    friend class Node;
    friend class EventFlow;

public:
    class Node
    {
        friend class EventFlow;
        
    protected:
        T* m_data;
        Node* m_parent;
        // TO-DO: Bug of gcc 4.8? Cannot compile with Node*.
        std::unordered_set<void*> m_children;
        Tree* m_tree;
        std::mutex m_mutex;

        bool _detachChild(Node* child)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_children.erase(child);
        }

        bool _attachChild(Node* child)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_children.insert(child).second;
        }

    public:
        Node(T* data, Node* parent, Tree* tree)
            : m_data(std::move(data)),
            m_parent(parent),
            m_tree(tree)
        {}

        virtual ~Node()
        {
            // Clear children when destruct.
            clearChild();

            if(m_data)
            {
                m_tree->m_dataFactory.recycle(m_data);
            }
        }

        T* getData()
        {
            return m_data;
        }

        template<typename P>
        P* getData()
        {
            P* naked = dynamic_cast<P*>(m_data);
            if(naked)
            {
                return naked;
            }
            else
            {
                throw exception::BadCast();
            }
        }

        Node* getParent() const
        {
            return m_parent;
        }

        void setParent(Node* newParent)
        {
            if(m_parent)
            {
                m_parent->_detachChild(this);
            }

            newParent->_attachChild(this);
            m_parent = newParent;
        }

        bool destroyChild(Node* child)
        {
            if(!_detachChild(child))
            {
                return false;
            }

            return m_tree->m_nodeFactory->recycle(child);
        }

        void clearChild()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto end = m_children.end();
            for(auto i = m_children.begin(); i != end; ++i)
            {
                // TO-DO: Remove static_cast.
                m_tree->m_nodeFactory.recycle(static_cast<Node*>(*i));
            }
            m_children.clear();
        }

        void destroySelf()
        {
            if(this == m_tree->m_root)
            {
                return m_tree->resetRoot();
            }

            m_tree->m_nodeFactory.recycle(this);
        }
    };

    Tree()
        : m_root(nullptr)
    {
    }

    virtual ~Tree()
    {
        m_nodeFactory.recycle(m_root);
    }

    Node* getRoot()
    {
        return m_root;
    }

    void resetRoot(Node* node)
    {
        m_nodeFactory.recycle(m_root);
        m_root = node;
    }

    void resetRoot()
    {
        m_nodeFactory.recycle(m_root);
        m_root = nullptr;
    }

    template<typename P, typename...Args>
    void resetRoot(Args...args)
    {
        m_nodeFactory.recycle(m_root);
        m_root = createNode<P>(std::forward<Args>(args)...);
    }

    template<typename P, typename...Args>
    Node* createNode(Args...args)
    {
        P* newData = m_dataFactory.template produce<P>(std::forward<Args>(args)...);
        return m_nodeFactory.template produce<Node>(newData, nullptr, this);
    }

protected:
    Factory<Node> m_nodeFactory;
    Factory<T> m_dataFactory;
    Node* m_root;
};

} // namespace backtrace

#endif
