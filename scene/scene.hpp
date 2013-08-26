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

class Scene
{
    friend class Root;

protected:
    Factory<GeometricObject> mObjectFactory;

public:
    virtual ~Scene() {}

    template<typename T, typename...Args>
    T* addObject(Args...args)
    {
        return mObjectFactory.produce<T>(std::forward<Args>(args)...);
    }

    void removeObject(GeometricObject* objectPtr)
    {
        mObjectFactory.recycle(objectPtr);
    }

    void hitObject(Ray* ray, ShadeRecord* rec)
    {
        double t;
        double tMin = kHugeValue;

        mObjectFactory.iterateProducts([&t, &tMin, ray, rec](const GeometricObject& obj) {
            if(obj.hit(ray, t, rec) && (t < tMin))
            {
                tMin = t;
                rec->hitAnObject = true;
                rec->color = obj.getColor();
            }
        });
    }
};

}

#endif
