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
#include <memory>
#include <cassert>

#include "../tree.hpp"

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

Tree<int> tree;

int main()
{
    tree.resetRoot<int>(0);

    auto root = tree.getRoot();
    auto node1 = tree.createNode<int>(1);
    auto node2 = tree.createNode<int>(2);
    auto node3 = tree.createNode<int>(3);
    auto node4 = tree.createNode<int>(4);
    auto node5 = tree.createNode<int>(5);
    auto node6 = tree.createNode<int>(6);

    cout << "Building structure..." << endl;

    node5->setParent(node2);
    node6->setParent(node2);
    node3->setParent(node1);
    node4->setParent(node1);
    node1->setParent(root);
    node2->setParent(root);

    /*
     *  Current Structure:
     * 
     *         Root
     *          /\
     *         /  \
     *      Node1  \
     *       / \   Node2
     *  Node3 Node4 / \
     *             /   \
     *          Node5 Node6
     */

    cout << "Relink node2 to node4." << endl;

    node2->setParent(node4);

    /*
     *  Current Structure:
     * 
     *         Root
     *          /
     *       Node1
     *        /\
     *   Node3 Node4
     *           \
     *          Node2
     *           / \
     *       Node5 Node6
     */

    cout << "Check parents." << endl;

    assert(node5->getParent() == node2);
    assert(node6->getParent() == node2);
    assert(node2->getParent() == node4);
    assert(node3->getParent() == node1);
    assert(node4->getParent() == node1);
    assert(node1->getParent() == root);

    cout << "Check data." << endl;

    assert(*node1->getData() == 1);
    assert(*node2->getData() == 2);
    assert(*node3->getData() == 3);
    assert(*node4->getData() == 4);
    assert(*node5->getData() == 5);
    assert(*node6->getData() == 6);

    cout << "Assign data." << endl;

    assert(*node1->getData() = 6);
    assert(*node2->getData() = 5);
    assert(*node3->getData() = 4);
    assert(*node4->getData() = 3);
    assert(*node5->getData() = 2);
    assert(*node6->getData() = 1);

    cout << "Check data again." << endl;

    assert(*node1->getData() == 6);
    assert(*node2->getData() == 5);
    assert(*node3->getData() == 4);
    assert(*node4->getData() == 3);
    assert(*node5->getData() == 2);
    assert(*node6->getData() == 1);

    cout << "Destory node 1. Should cause 6 destructions." << endl;

    node1->destroySelf();

    cout << "Do not use any of those nodes now." << endl;

    cout << "Destory root." << endl;

    root->destroySelf();

    cout << "All passed." << endl;
}
