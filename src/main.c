#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "SDL3/SDL.h"

#include "utils.h"
#include "state.h"
#include "controller.h"
#include "renderer.h"

int main()
{
    SDL_Window *sdl_window = SDL_CreateWindow("ethereal-dust", 640, 480, 0);
    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
    SDL_SetRenderVSync(sdl_renderer, 1);

    ETHER_state state;
    state.sdl_window = sdl_window;
    state.sdl_renderer = sdl_renderer;
    state.quit = ETHER_FALSE;

    while (state.quit == ETHER_FALSE)
    {
        ETHER_update(&state);
        ETHER_render(&state);
    }

    SDL_DestroyWindow(sdl_window);
    SDL_DestroyRenderer(sdl_renderer);

    return 0;
}