#include "renderer.h"

#define ETHER_DEBUG_COLLISION_LINES

void ETHER_render(ETHER_state *state)
{

#ifndef ETHER_DEBUG_COLLISION_LINES
    SDL_SetRenderDrawColor(state->sdl_renderer, 25, 25, 25, 255);
    SDL_RenderClear(state->sdl_renderer);
#endif

    ETHER_render_entities(state);
    ETHER_render_blocks(state);

    SDL_RenderPresent(state->sdl_renderer);

    // printf("%d\n", state->stats->money);
}

void ETHER_render_entities(ETHER_state *state)
{
    // this can be optimized with SDL_FillSurfaceRects
    for (ETHER_entity_id_t i = 0; i < state->entities->len; i++)
    {
        SDL_SetRenderDrawColor(state->sdl_renderer, 225, 255, 225, 255);
        SDL_FRect sdl_rect;
        sdl_rect.x = state->entities->transforms[i].pos.x;
        sdl_rect.y = state->entities->transforms[i].pos.y;
        sdl_rect.w = ETHER_ENTITY_SIZE;
        sdl_rect.h = ETHER_ENTITY_SIZE;
        SDL_RenderFillRect(state->sdl_renderer, &sdl_rect);
    }
}

void ETHER_render_blocks(ETHER_state *state)
{
    // this can be optimized with SDL_FillSurfaceRects
    for (ETHER_block_id_t i = 0; i < state->blocks->len; i++)
    {
        if (state->blocks->counts[i] <= 0) continue;
        SDL_SetRenderDrawColor(state->sdl_renderer, 225, 255, 225, 255);
        SDL_FRect sdl_rect;
        sdl_rect.x = state->blocks->rects[i].x;
        sdl_rect.y = state->blocks->rects[i].y;
        sdl_rect.w = state->blocks->rects[i].w;
        sdl_rect.h = state->blocks->rects[i].h;
        SDL_RenderFillRect(state->sdl_renderer, &sdl_rect);
        SDL_SetRenderDrawColor(state->sdl_renderer, 25, 25, 25, 255);
        char num[16] = {0};
        snprintf(num, 16, "%u", state->blocks->counts[i]);
        SDL_RenderDebugText(state->sdl_renderer, sdl_rect.x + 10, sdl_rect.y + 10, num);
    }
}
