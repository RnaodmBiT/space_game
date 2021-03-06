#include <cstdio>
#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <chaiscript/chaiscript.hpp>
#include <maths.hpp>
#include <rendering.hpp>
#include <io.hpp>
#include <nodes/camera.hpp>
#include <nodes/skybox.hpp>

#define WINDOW_TITLE        "Model Viewer"


class model : public node {
    mesh& m;
    gl::program& s;
public:
    model(mesh& m, gl::program& s) : m(m), s(s) { }

    void draw(render_state rs) {
        s.use();
        s["transform"] = rs.projection * rs.view;
        s["world"] = rs.world;
        m.draw(s["albedo"], s["roughness"], s["metalness"]);

        node::draw(rs);
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

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);


    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              options.width,
                              options.height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);

    glewExperimental = true;
    assert(glewInit() == GLEW_OK && "Error initializing GLEW");

    glEnable(GL_DEPTH_TEST);


    node root;
    camera viewport;
    root.add(&viewport);

    viewport.position = { -5.0f, 3.0f, -3.0f };
    viewport.direction = { 5.0f, 1.0f, 3.0f };
    viewport.up = { 0.0f, 1.0f, 0.0f };

    skybox sky;
    sky.load_cube_map("front.bmp", "back.bmp", "top.bmp", "bottom.bmp", "left.bmp", "right.bmp");
    sky.load_shader("shaders/skybox.vert", "shaders/skybox.frag");
    viewport.add(&sky);

    mesh m;
    m.load_file("models/cube.dae");

    gl::program s;
    gl::shader vertex(gl::shader::vertex), fragment(gl::shader::fragment);
    vertex.set_source(file::read("shaders/object.vert"));
    fragment.set_source(file::read("shaders/object.frag"));
    s.build(vertex, fragment);

    model object(m, s);
    viewport.add(&object);

    float time = 0;

    running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    glViewport(0, 0, event.window.data1, event.window.data2);
                    viewport.aspect = float(event.window.data1) / float(event.window.data2);
                }
                break;

            default:
                break;
            }
        }

        time += 0.01f;
        viewport.position.x = 10 * cos(time);
        viewport.position.z = 10 * sin(time);
        viewport.direction = -viewport.position;

        s.use();
        s["camera_position"] = viewport.position;

        /* TODO: Update the game logic here. */
        root.update(1.0f);

        /* TODO: Render the game to the screen here. */
        root.draw();


        SDL_GL_SwapWindow(window);
    }
    SDL_Quit();

    return 0;
}
