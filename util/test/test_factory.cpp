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
#include <chrono>
#include <memory>
#include <thread>
#include <cassert>

#include "../factory.hpp"

using namespace std;
using namespace backtrace;

class A
{
public:
    A() { cout << "constructA" << endl; }
    virtual ~A() { cout << "destructA" << endl; }
};

class B : public A
{
public:
    B(int, int, int) { cout << "constructB" << endl; }
    virtual ~B() { cout << "destructB" << endl; }
};

Factory<A> factory;

void prevent(B* naked)
{
    assert(factory.protect(naked) == true);
    cout << "protect" << endl;
    cout << "will sleep for 2s" << endl;
    chrono::milliseconds dura(2000);
    this_thread::sleep_for(dura);
    assert(factory.unprotect(naked) == true);
    cout << "unprotect" << endl;
}

int main()
{    
    B* naked = factory.produce<B>(5, 5, 5);
    assert(factory.isAlive(naked) == true);
    assert(factory.isProtected(naked) == false);
    assert(factory.isPendingDelete(naked) == false);
    unique_ptr<thread> th(new thread(&prevent, naked));
    chrono::milliseconds dura(200);
    this_thread::sleep_for(dura);
    assert(factory.recycle(naked) == false);
    assert(factory.isProtected(naked) == true);
    assert(factory.isPendingDelete(naked) == true);
    th->join();
    assert(factory.recycle(naked) == false);
    assert(factory.protect(naked) == false);
    assert(factory.unprotect(naked) == false);
    assert(factory.isAlive(naked) == false);
}
