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

#ifndef BACKTRACE_SCENEMANAGERBASE_H
#define BACKTRACE_SCENEMANAGERBASE_H

#include <unordered_set>

#include "../object/geometricobject.hpp"
#include "../util/factory.hpp"

namespace backtrace {

class SceneManager
{
    friend class Root;

protected:
    Factory<GeometricObject> mObjectFactory;
    std::unordered_set<GeometricObject*> mObjects;

public:
    virtual ~SceneManager() {}

    template<typename T, typename...Args>
    T* addObject(Args...args)
    {
        T* naked = mObjectFactory.produce<T>(std::forward<Args>(args)...);
        mObjects.insert(naked);
        return naked;
    }

    void removeObject(GeometricObject* objectPtr)
    {
        mObjectFactory.recycle(objectPtr);
        mObjects.erase(objectPtr);
    }

    bool hitObject(const Ray& ray, ShadeRecord& rec)
    {
        double t;
        double tMin = kHugeValue;

        auto end = mObjects.end();
        for(auto iter = mObjects.begin(); iter != end; ++iter)
        {
            if((*iter)->hit(ray, t, rec) && (t < tMin))
            {
                tMin = t;
                rec.hitAnObject = true;
                rec.color = (*iter)->getColor();
            }
        }

        return rec.hitAnObject;
    }
};

}

#endif
