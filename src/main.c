#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "SDL3/SDL.h"

#include "utils.h"
#include "state.h"
#include "controller.h"
#include "renderer.h"

int main()
{
    srand(time(NULL));

    SDL_Window *sdl_window = SDL_CreateWindow("ethereal-dust", ETHER_SCREEN_WIDTH, ETHER_SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
    SDL_SetRenderVSync(sdl_renderer, 1);

    ETHER_entities entities;
    entities.len = 16;
    entities.cap = 16;
    entities.transforms = malloc(entities.cap * sizeof(*entities.transforms));

    for (ETHER_entity_id_t i = 0; i < entities.len; i++)
    {
        ETHER_vec pos;
        pos.x = rand() % (ETHER_WORLD_WIDTH - ETHER_ENTITY_SIZE);
        pos.y = rand() % (ETHER_WORLD_HEIGHT - ETHER_ENTITY_SIZE);
        ETHER_vec vel;
        // constant taxicab speed
        uint16_t v = 32;
        uint16_t vx = 16;//rand() % v;
        uint16_t vy = v - vx;
        vel.x = vx * (1 - 2 * (rand() % 2));
        vel.y = vy * (1 - 2 * (rand() % 2));
        ETHER_entity_transform transform;
        transform.pos = pos;
        transform.vel = vel;
        entities.transforms[i] = transform;
    }

    ETHER_blocks blocks;
    blocks.len = 128;
    blocks.cap = 128;
    blocks.rects = malloc(blocks.cap * sizeof(*blocks.rects));
    blocks.counts = malloc(blocks.cap * sizeof(*blocks.counts));
    
    srand(time(NULL));

    // lol kinda hacky approach to world borders, but it works i guess :P
    // maybe there could be an easter egg where the balls break out of the
    // world borders...

    blocks.counts[0] = UINT32_MAX;
    blocks.counts[1] = UINT32_MAX;
    blocks.counts[2] = UINT32_MAX;
    blocks.counts[3] = UINT32_MAX;
    blocks.rects[0] = (ETHER_rect) { 0, 0, 0, ETHER_WORLD_HEIGHT };
    blocks.rects[1] = (ETHER_rect) { 0, 0, ETHER_WORLD_WIDTH, 0 };
    blocks.rects[2] = (ETHER_rect) { 0, ETHER_WORLD_HEIGHT, ETHER_WORLD_WIDTH, 0 };
    blocks.rects[3] = (ETHER_rect) { ETHER_WORLD_WIDTH, 0, 0, ETHER_WORLD_HEIGHT };

    for (ETHER_block_id_t i = 4; i < blocks.len; i++)
    {
        ETHER_rect rect;
        rect.x = (rand() % (ETHER_WORLD_WIDTH / ETHER_BLOCK_WIDTH)) * ETHER_BLOCK_WIDTH;
        rect.y = (rand() % (ETHER_WORLD_HEIGHT / ETHER_BLOCK_HEIGHT)) * ETHER_BLOCK_HEIGHT;
        rect.w = ETHER_BLOCK_WIDTH;
        rect.h = ETHER_BLOCK_HEIGHT;
        ETHER_block_count_t count = 100;
        blocks.rects[i] = rect;
        blocks.counts[i] = count;
    }

    ETHER_state state;
    state.sdl_window = sdl_window;
    state.sdl_renderer = sdl_renderer;
    state.quit = ETHER_FALSE;
    state.tick = 0;
    state.entities = &entities;
    state.blocks = &blocks;

    while (state.quit == ETHER_FALSE)
    {
        ETHER_update(&state);
        ETHER_render(&state);
    }

    SDL_DestroyWindow(sdl_window);
    SDL_DestroyRenderer(sdl_renderer);

    return 0;
}