#ifndef STATE_H
#define STATE_H

#include "SDL3/SDL.h"

#include "utils.h"

typedef struct ETHER_state ETHER_state;

struct ETHER_state
{
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    ETHER_bool quit;
};

#endif