#pragma once

#include "GL/glew.h"
#include <cassert>
#include <string>
#include <cstdio>
#include <stb_image.h>
#include <map>
#include <vector>
#include "maths.hpp"

namespace gl {

    class cube_map {
        GLuint t;

        void load_side(GLenum side_target, const std::string& filename) {
            bind();

            int w, h, channels;
            uint8_t* image = stbi_load(filename.c_str(), &w, &h, &channels, 4);
            assert(image && "Error loading cube map image");

            glTexImage2D(side_target, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

            stbi_image_free(image);
        }

    public:
        cube_map() {
            glGenTextures(1, &t);
        }

        ~cube_map() {
            if (t >= 0) {
                glDeleteTextures(1, &t);
            }
        }

        cube_map(cube_map&& move) : t(-1) {
            std::swap(t, move.t);
        }

        void operator=(cube_map&& move) {
            std::swap(t, move.t);
        }

        void bind() const {
            glBindTexture(GL_TEXTURE_CUBE_MAP, t);
        }

        void load_cube(const std::string& front,
                       const std::string& back,
                       const std::string& top,
                       const std::string& bottom,
                       const std::string& left,
                       const std::string& right) {

            load_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
            load_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
            load_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
            load_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
            load_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
            load_side(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
    };

    class shader {
        GLuint s;
        friend class program;
    public:
        enum type { vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER };

        shader(type t) {
            s = glCreateShader(t);
        }

        ~shader() {
            if (s >= 0) {
                glDeleteShader(s);
            }
        }

        shader(shader&& move) : s(-1) {
            std::swap(s, move.s);
        }

        void operator=(shader&& move) {
            std::swap(s, move.s);
        }

        void set_source(const std::string& source) {
            const char* str = source.c_str();
            int len = source.size();
            glShaderSource(s, 1, &str, &len);

            glCompileShader(s);

            int compiled;
            glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);

            if (!compiled) {
                int length;
                glGetShaderiv(s, GL_INFO_LOG_LENGTH, &length);

                std::string log(length, ' ');
                glGetShaderInfoLog(s, length, &length, &log[0]);
                printf("Shader log: %s\n", log.c_str());
                assert(false && "Error compiling shader");
            }
        }
    };

    class uniform {
        GLuint id;
    public:
        uniform(GLuint id = -1) : id(id) { }

        void set(const mat4& m) {
            glUniformMatrix4fv(id, 1, true, m.data.data());
        }

        void set(float x) {
            glUniform1f(id, x);
        }

        void set(const vec2& v) {
            glUniform2f(id, v.x, v.y);
        }

        void set(const vec3& v) {
            glUniform3f(id, v.x, v.y, v.z);
        }

        void set(const vec4& v) {
            glUniform4f(id, v.x, v.y, v.z, v.w);
        }

        void set(const cube_map& cube) {
            glUniform1i(id, 0); // TODO: Work out how to deal with more than one texture
            glActiveTexture(GL_TEXTURE0);
            cube.bind();
        }

        template <typename T>
        void operator=(const T& value) {
            set(value);
        }
    };

    class program {
        GLuint p;
        std::map<std::string, uniform> uniforms;
    public:
        program() {
            p = glCreateProgram();
        }

        ~program() {
            if (p >= 0) {
                glDeleteProgram(p);
            }
        }

        program(program&& move) : p(-1) {
            std::swap(p, move.p);
        }

        void operator=(program&& move) {
            std::swap(p, move.p);
        }

        void attach(const shader& s) {
            glAttachShader(p, s.s);
        }

        /* Attach two shaders and link them together. */
        void build(const shader& a, const shader& b) {
            attach(a);
            attach(b);
            link();
        }

        void link() {
            glLinkProgram(p);

            int linked;
            glGetProgramiv(p, GL_LINK_STATUS, &linked);

            if (!linked) {
                int length;
                glGetProgramiv(p, GL_INFO_LOG_LENGTH, &length);

                std::string log(length, ' ');
                glGetShaderInfoLog(p, length, &length, &log[0]);
                printf("Program log: %s\n", log.c_str());
                assert(false && "Error linking program");
            }
        }

        void use() {
            glUseProgram(p);
        }

        uniform& get(const std::string& name) {
            if (!uniforms.count(name)) {
                uniforms[name] = uniform(glGetUniformLocation(p, name.c_str()));
            }
            return uniforms[name];
        }

        uniform& operator[](const std::string& name) {
            return get(name);
        }
    };

    class buffer {
    public:
        enum type { array = GL_ARRAY_BUFFER, element = GL_ELEMENT_ARRAY_BUFFER };
    private:
        GLuint b;
        type t;
    public:

        buffer(type t = array) : t(t) {
            glGenBuffers(1, &b);
        }

        ~buffer() {
            if (b >= 0) {
                glDeleteBuffers(1, &b);
            }
        }

        buffer(buffer&& move) : b(-1) {
            std::swap(b, move.b);
        }

        void operator=(buffer&& move) {
            std::swap(b, move.b);
        }

        void bind() {
            glBindBuffer(t, b);
        }

        template <typename T>
        void set_data(const T* data, int count) {
            bind();
            glBufferData(t, sizeof(T) * count, data, GL_STATIC_DRAW);
        }

        template <typename T>
        void set_data(const std::vector<T>& data) {
            set_data(data.data(), (int)data.size());
        }
    };

    class array {
        GLuint a;
        int attributes;
    public:
        array() : attributes(0) {
            glGenVertexArrays(1, &a);
        }

        ~array() {
            if (a >= 0) {
                glDeleteVertexArrays(1, &a);
            }
        }

        array(array&& move) : a(-1) {
            std::swap(a, move.a);
        }

        void operator=(array&& move) {
            std::swap(a, move.a);
        }

        void bind() const {
            glBindVertexArray(a);
        }

        void attach(buffer& b, int components, GLenum data_type, bool normalized) {
            bind();

            glEnableVertexAttribArray(attributes);

            b.bind();
            glVertexAttribPointer(attributes, components, data_type, normalized, 0, 0);

            attributes++;
        }

        void draw(int vertices, GLenum primitive_type = GL_TRIANGLES) const {
            bind();
            glDrawArrays(primitive_type, 0, vertices);
        }
    };

}

// A mesh is a renderable object, usually imported from a file
struct mesh {
    struct part {
        // graphical model data
        std::vector<gl::buffer> buffers;
        gl::array object;
        int vertices;

        // material properties
        vec3 albedo;
        float roughness;
        float metalness;

        // A helper function to quickly add a buffer of data to this part
        template <typename T>
        void add_buffer(const std::vector<T>& data, int components, GLenum type, bool normalized) {
            gl::buffer b;
            b.set_data(data);
            object.attach(b, components, type, normalized);
            buffers.push_back(std::move(b));
        }
    };

    std::vector<part> parts;

    // create a new empty mesh object
    mesh() { }

    // create a new mesh object from a file
    mesh(const std::string& filename) {
        load_file(filename);
    }

    mesh(mesh&& move) {
        std::swap(parts, move.parts);
    }

    void operator=(mesh&& move) {
        std::swap(parts, move.parts);
    }

    // Load a model from a file
    void load_file(const std::string& filename);

    // Generate a model from a list of vertices, a list of normals, and a list of indices. Each index is a pair into the list of vertices and list of normals. The indices are read in threes to generate trigangles.
    void generate_part(const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<std::pair<int, int>>& indices, const vec3& albedo, float roughness, float metalness);

    void draw(gl::uniform& albedo, gl::uniform& roughness, gl::uniform& metalness) const {
        for (const part& p : parts) {
            albedo = p.albedo;
            roughness = p.roughness;
            metalness = p.metalness;

            p.object.draw(p.vertices);
        }
    }

    // Common geometry shapes
    static mesh cube();
};


