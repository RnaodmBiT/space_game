#pragma once
#include <node.hpp>
#include <maths.hpp>
#include <GL/glew.h>

class camera : public node {
public:
    vec3 position, direction, up;
    float fov, aspect;
    float near_plane, far_plane;

    camera() : fov(pi / 4), aspect(1), near_plane(1), far_plane(100) { }

    mat4 view() const {
        return look_at(position, position + direction, up);
    }

    mat4 projection() const {
        return perspective(fov, aspect, near_plane, far_plane);
    }

    void draw(render_state rs) {
        glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rs.projection = projection();
        rs.view = view();
        rs.world = mat4();

        node::draw(rs);
    }
};

