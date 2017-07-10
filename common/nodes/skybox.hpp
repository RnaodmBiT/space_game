#pragma once
#include <node.hpp>
#include <maths.hpp>
#include <rendering.hpp>

class skybox : public node {

    mesh box;
    gl::program shader;
    gl::cube_map cube;

public:

    skybox() { }

    void load_box(const std::string& filename) {
        box.load_file(filename);
    }
    
    void load_shader(const std::string& vertex_shader, const std::string& fragment_shader) {
        gl::shader vertex(gl::shader::vertex), fragment(gl::shader::fragment);
        vertex.set_source(file::read(vertex_shader));
        fragment.set_source(file::read(fragment_shader));

        shader.build(vertex, fragment);
    }

    void load_cube_map(const std::string& front, const std::string& back, const std::string& top, const std::string& bottom, const std::string& left, const std::string& right) {
        cube.load_cube(front, back, top, bottom, left, right);
    }

    void draw(render_state rs) {

        rs.world = mat4();
        rs.view(0, 3) = rs.view(1, 3) = rs.view(2, 3) = 0.0f; // Remove the movement of the camera.

        glDisable(GL_DEPTH_TEST); // TODO: Some kind of state class to tidy this up.

        shader.use();

        shader["transform"] = rs.projection * rs.view;
        shader["world"] = rs.world;
        shader["cube"] = cube;
        box.draw(shader["albedo"], shader["roughness"], shader["metalness"]);

        node::draw();

        glEnable(GL_DEPTH_TEST);
    }
};

