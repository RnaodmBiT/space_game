#include <cstdio>
#include <SDL.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <v7/v7.h>

#define WINDOW_TITLE        "Space"

/* game_options holds all of the current settings applied to the game.
 */
struct game_options {
    int width, height; /* The current operating resolution. */
};


v7_err println(v7* ctx, v7_val_t* result) {
    v7_val_t val = v7_arg(ctx, 0);
    const char* str = v7_get_string(ctx, &val, nullptr);
    printf("%s\n", str);
    return V7_OK;
}


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

    v7* ctx = v7_create();
    v7_set_method(ctx, v7_get_global(ctx), "print", println);

    v7_val_t result;
    v7_err err = v7_exec(ctx, "print('Hello World')", &result);
    err = v7_exec_file(ctx, "test.js", &result);

    v7_val_t function = v7_get(ctx, v7_get_global(ctx), "test", ~0);
    err = v7_apply(ctx, function, V7_UNDEFINED, V7_UNDEFINED, &result);

    int is = v7_is_object(result);

    prop_iter_ctx iter_ctx;
    v7_val_t name, val;
    v7_prop_attr_t attrs;
    
    v7_init_prop_iter_ctx(ctx, result, &iter_ctx);
    while (v7_next_prop(ctx, &iter_ctx, &name, &val, &attrs)) {
        if (v7_is_string(val)) {
            printf("string: %s = %s", v7_get_string(ctx, &name, nullptr), v7_get_string(ctx, &val, nullptr));
        }
    }
    v7_destruct_prop_iter_ctx(ctx, &iter_ctx);

    v7_destroy(ctx);

    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              options.width,
                              options.height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);

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

        /* TODO: Render the game to the screen here. */

        SDL_GL_SwapWindow(window);
    }
    SDL_Quit();

    return 0;
}
