#include "controller.h"

void ETHER_update(ETHER_state *state)
{
    ETHER_update_events(state);
}

void ETHER_update_events(ETHER_state *state)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ETHER_update_event(state, &event);
    }
}

void ETHER_update_event(ETHER_state *state, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        state->quit = ETHER_TRUE;
    }
}