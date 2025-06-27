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
    ETHER_bool smth;
    ETHER_bool smth_state;
    ETHER_bool smth_last;
    uint32_t tick;
    struct ETHER_entities *entities;
    struct ETHER_blocks *blocks;
    struct ETHER_stats *stats;
};

/* stats */

typedef struct ETHER_stats ETHER_stats;
typedef unsigned int ETHER_level_t;
typedef unsigned int ETHER_money_t;

struct ETHER_stats
{
    ETHER_level_t level;
    ETHER_money_t money;
};

/* entities */

typedef uint16_t ETHER_entity_id_t;
typedef struct ETHER_entities ETHER_entities;
typedef struct ETHER_entity_transform ETHER_entity_transform;

#define ETHER_ENTITY_SIZE 16

struct ETHER_entities
{
    ETHER_entity_id_t len;
    ETHER_entity_id_t cap;

    ETHER_entity_transform *transforms;
};

struct ETHER_entity_transform
{
    ETHER_vec pos;
    ETHER_vec vel;
};

/* blocks */

typedef int ETHER_block_id_t;
typedef struct ETHER_blocks ETHER_blocks;

#define ETHER_BLOCK_WIDTH 64
#define ETHER_BLOCK_HEIGHT 32

struct ETHER_blocks
{
    ETHER_block_id_t len;
    ETHER_block_id_t cap;

    ETHER_rect *rects;
    ETHER_level_t *counts;
};

#endif
