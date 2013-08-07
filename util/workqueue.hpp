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

#ifndef BACKTRACE_WORK_QUEUE_H
#define BACKTRACE_WORK_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <memory>
#include <vector>
#include <queue>

#include "factory.hpp"

namespace backtrace
{
    class WorkQueue
    {
    protected:
        std::queue<std::function<void()>> m_jobs;
        std::vector<std::unique_ptr<std::thread>> m_workers;
        std::condition_variable m_condition;
        std::mutex m_conditionMutex;
        std::mutex m_queueMutex;
        bool m_continue;

        void _process()
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            while(!m_jobs.empty())
            {
                std::function<void()> op = m_jobs.front();
                m_jobs.pop();
                lock.unlock();
                op();
                lock.lock();
            }
        }

        void _worker()
        {
            while(m_continue)
            {
                std::unique_lock<std::mutex> lock(m_conditionMutex);
                m_condition.wait(lock);
                lock.unlock();

                _process();
            }
        }

    public:
        WorkQueue(size_t workers)
            : m_continue(true)
        {
            for(size_t i = 0; i < workers; ++i)
            {
                m_workers.push_back(std::unique_ptr<std::thread>(new std::thread(&WorkQueue::_worker, this)));
            }
        }

        virtual ~WorkQueue()
        {
            m_continue = false;
            m_condition.notify_all();
            joinAll();
        }

        void push(std::function<void()> work)
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_jobs.push(std::move(work));
            m_condition.notify_one();
        }

        void joinAll()
        {
            auto end = m_workers.end();
            for(auto i = m_workers.begin(); i != end; ++i)
            {
                if((*i)->joinable())
                {
                    (*i)->join();
                }
            }
        }
    };

} // namespace backtrace

#endif
