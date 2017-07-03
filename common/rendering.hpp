#pragma once

#include "GL/glew.h"
#include <cassert>
#include <string>

namespace gl {

    class shader {
        GLuint s;
        friend class program;
    public:
        enum type { vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER };

        shader(type t) {
            s = glCreateShader(t);
        }

        ~shader() {
            glDeleteShader(s);
        }

        shader(shader&& move) {
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
    };

    class program {
        GLuint p;
        std::map<std::string, uniform> uniforms;
    public:
        program() {
            p = glCreateProgram();
        }

        ~program() {
            glDeleteProgram(p);
        }

        program(program&& move) {
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
            glDeleteBuffers(1, &b);
        }

        buffer(buffer&& move) {
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
            glDeleteVertexArrays(1, &a);
        }

        array(array&& move) {
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
