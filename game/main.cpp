#include <cstdio>
#include <SDL.h>
#include <GL\glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <chaiscript/chaiscript.hpp>
#include <maths.hpp>
#include <rendering.hpp>
#include <io.hpp>
#include "node.hpp"

#define WINDOW_TITLE        "Space"

using namespace chaiscript;

class camera : public node {
public:
    vec3 position, direction, up;

    mat4 transform() const {
        return perspective(pi / 4, 1280.f / 720.f, 1.f, 100.f) * look_at(position, position + direction, up);
    }

    void draw() {
        glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        node::draw();
    }
};


/* game_options holds all of the current settings applied to the game.
 */
struct game_options {
    int width, height; /* The current operating resolution. */
};

/* The main entry point into the game.
*/
int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;
    bool running;


    game_options options = {
        1280,
        720
    };

    /* TODO: Parse the command line and configuration file options. */

    SDL_Init(SDL_INIT_EVERYTHING);

    ChaiScript chai;
    chai.use("test.chai");

    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              options.width,
                              options.height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);

    glewExperimental = true;
    assert(glewInit() == GLEW_OK && "Error initializing GLEW");

    gl::shader vertex(gl::shader::vertex), fragment(gl::shader::fragment);
    vertex.set_source(file::read("shaders/object.vert"));
    fragment.set_source(file::read("shaders/object.frag"));

    gl::program shader;
    shader.build(vertex, fragment);
    shader.use();

    std::vector<vec3> verts = {
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { 0.f, 1.f, 0.f }
    };

    gl::buffer vertex_buffer;
    vertex_buffer.set_data(verts);

    gl::array object;
    object.attach(vertex_buffer, 3, GL_FLOAT, false);

    node root;
    camera viewport;
    root.add(&viewport);

    viewport.position = { -5.0f, 2.0f, -3.0f };
    viewport.direction = { 5.0f, -2.0f, 3.0f };
    viewport.up = { 0.0f, 1.0f, 0.0f };

    shader.get("transform").set(viewport.transform());
    shader.get("world").set(mat4());

    running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            default:
                break;
            }
        }

        /* TODO: Update the game logic here. */
        root.update(1.0f);

        /* TODO: Render the game to the screen here. */
        root.draw();

        object.draw(verts.size());

        SDL_GL_SwapWindow(window);
    }
    SDL_Quit();

    return 0;
}
