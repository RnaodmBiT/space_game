#pragma once
#include <node.hpp>
#include <maths.hpp>
#include <GL/glew.h>

class camera : public node {
public:
    vec3 position, direction, up;

    mat4 transform() const {
        return perspective(pi / 4, 1280.f / 720.f, 1.f, 100.f) * look_at(position, position + direction, up);
    }

    void draw() {
        glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        node::draw();
    }
};
