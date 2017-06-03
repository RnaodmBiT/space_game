#include <cstdio>
#include <SDL.h>

#define WINDOW_TITLE        "Space"

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
