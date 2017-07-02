#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>

class node {
    std::vector<node*> children;
    node* parent;

public:

    node() : parent(nullptr) { }

    void add(node* child) {
        assert(child->parent == nullptr && "child already has a parent");
        children.push_back(child);
        child->parent = child;
    }

    void remove(node* child) {
        assert(child->parent == this && "child doesn't belong to this node");
        children.erase(std::remove(children.begin(), children.end(), child));
        child->parent = nullptr;
    }

    node* find(std::function<bool(node*)> fn) const {
        auto it = std::find_if(children.begin(), children.end(), fn);
        return it == children.end() ? nullptr : *it;
    }

    node* get_parent() const {
        return parent;
    }

    virtual void update(float dt) {
        for (node* n : children) {
            n->update(dt);
        }
    }

    virtual void draw() {
        for (node* n : children) {
            n->draw();
        }
    }
};
