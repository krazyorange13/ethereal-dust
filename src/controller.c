#include "controller.h"

void ETHER_update(ETHER_state *state)
{
    ETHER_update_events(state);
    ETHER_update_entities_and_blocks(state);
    state->tick++;
}
void ETHER_update_events(ETHER_state *state)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ETHER_update_event(state, &event);
    }

    ETHER_update_mouse(state);
}

void ETHER_update_mouse(ETHER_state *state)
{
    SDL_GetMouseState(&state->mouse_x, &state->mouse_y);
}


void ETHER_update_event(ETHER_state *state, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        state->quit = ETHER_TRUE;
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.scancode == SDL_SCANCODE_Q)
        {
            state->quit = ETHER_TRUE;
        }
    }

    // stupid buggy macos input (next commit? lol ;-;)
    const bool *keys = SDL_GetKeyboardState(NULL);
    state->smth = keys[SDL_SCANCODE_SPACE];
    // state->smth = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
}

ETHER_rect ETHER_get_supersweep(ETHER_rect rect, ETHER_vec vel)
{
    return (ETHER_rect)
    {
        rect.x - ABS(vel.x),
        rect.y - ABS(vel.y),
        rect.w + ABS(vel.x) * 2,
        rect.h + ABS(vel.y) * 2
    };
}

ETHER_rect ETHER_get_sweep(ETHER_rect rect, ETHER_vec vel)
{
    return (ETHER_rect)
    {
        MIN(rect.x, rect.x + vel.x),
        MIN(rect.y, rect.y + vel.y),
        MAX(rect.x + rect.w, rect.x + rect.w + vel.x) - MIN(rect.x, rect.x + vel.x),
        MAX(rect.y + rect.h, rect.y + rect.h + vel.y) - MIN(rect.y, rect.y + vel.y)
    };
}

#define FRECT(R) (SDL_FRect) {R.x, R.y, R.w, R.h}
#define FRECT_MID(R) (SDL_FRect) {R.x + ETHER_WORLD_WIDTH / 2, R.y + ETHER_WORLD_HEIGHT / 2, R.w, R.h}

#define ETHER_DEBUG_COLLISION_LINES // ALSO CHANGE renderer.c ETHER_DEBUG_COLLISION_LINES
// #define ETHER_DEBUG_COLLISION_STEP
#define ETHER_DEBUG_COLLISION_SMOOTH
#define ETHER_DEBUG_COLLISION_MOUSE

void ETHER_update_entities_and_blocks(ETHER_state *state)
{
#ifdef ETHER_DEBUG_COLLISION_LINES
    SDL_SetRenderDrawColor(state->sdl_renderer, 25, 25, 25, 255);
    SDL_RenderClear(state->sdl_renderer);

    SDL_FRect frect;
#endif

    for (ETHER_entity_id_t i = 0; i < state->entities->len; i++)
    {
        // if (state->smth)
        //     printf("hi im entity %d\n", i);

        ETHER_vec pos = state->entities->transforms[i].pos;
        ETHER_vec vel = state->entities->transforms[i].vel;
        ETHER_rect rect = {pos.x, pos.y, ETHER_ENTITY_SIZE, ETHER_ENTITY_SIZE};

        // frect = FRECT(rect);
        // SDL_SetRenderDrawColor(state->sdl_renderer, 0, 0, 255, 255);
        // SDL_RenderRect(state->sdl_renderer, &frect);
        
        ETHER_rect supersweep = ETHER_get_supersweep(rect, vel);
        ETHER_block_id_t supersweep_blocks[state->blocks->len];
        ETHER_block_id_t supersweep_blocks_len = 0;
        for (ETHER_block_id_t k = 0; k < state->blocks->len; k++)
            if (ETHER_rects_collide(supersweep, state->blocks->rects[k]) && state->blocks->counts[k] > 0)
                supersweep_blocks[supersweep_blocks_len++] = k;

#ifdef ETHER_DEBUG_COLLISION_LINES
        frect = FRECT(supersweep);
        SDL_SetRenderDrawColor(state->sdl_renderer, 255, 0, 255, 255);
        SDL_RenderRect(state->sdl_renderer, &frect);
#endif
        
        signed char vel_sign_x = 1;
        signed char vel_sign_y = 1;

        for (int j = 0; vel.x != 0 && vel.y != 0; j++)
        {

            // if (state->smth)
            //     printf("\titer %d\n", j);

            ETHER_rect sweep = ETHER_get_sweep(rect, vel);
            ETHER_block_id_t sweep_blocks[supersweep_blocks_len];
            ETHER_block_id_t sweep_blocks_len = 0;
            for (ETHER_block_id_t k = 0; k < supersweep_blocks_len; k++)
                if (ETHER_rects_collide(sweep, state->blocks->rects[supersweep_blocks[k]]))
                    sweep_blocks[sweep_blocks_len++] = supersweep_blocks[k];

#ifdef ETHER_DEBUG_COLLISION_LINES
            frect = FRECT(sweep);
            SDL_SetRenderDrawColor(state->sdl_renderer, 255, 0, 255, 255);
            SDL_RenderRect(state->sdl_renderer, &frect);
#endif
            
            ETHER_intersection_data hit = {1, ETHER_INTERSECTION_SIDE_NONE};
            ETHER_block_id_t hit_block = -1;

            for (ETHER_block_id_t k = 0; k < sweep_blocks_len; k++)
            {

                // if (state->smth)
                //     printf("\t\tblock %d\n", sweep_blocks[k]);

                ETHER_rect md = ETHER_rects_minkowski_difference(rect, state->blocks->rects[sweep_blocks[k]]);
                
                // frect = FRECT_MID(md);
                // SDL_SetRenderDrawColor(state->sdl_renderer, 150, 50, 50, 255);
                // SDL_RenderRect(state->sdl_renderer, &frect);

                ETHER_line relvel = {0, 0, -vel.x, -vel.y};

                // SDL_SetRenderDrawColor(state->sdl_renderer, 150, 50, 50, 255);
                // SDL_RenderLine(state->sdl_renderer, ETHER_WORLD_WIDTH / 2, ETHER_WORLD_HEIGHT / 2, relvel.x2 + ETHER_WORLD_WIDTH / 2, relvel.y2 + ETHER_WORLD_HEIGHT / 2);

                ETHER_intersection_data _hit = ETHER_rect_line_intersection(md, relvel);
                if (_hit.t < hit.t)
                {
                    hit = _hit;
                    hit_block = sweep_blocks[k];
                }

                // if (_hit.t < INFINITY || ETHER_vec_in_rect((ETHER_vec) {0, 0}, md))
                // {

                //     if (state->smth)
                //         printf("\t\tblock %d hit\n", sweep_blocks[k]);
                //     frect = FRECT(state->blocks->rects[sweep_blocks[k]]);
                //     SDL_SetRenderDrawColor(state->sdl_renderer, 225, 255, 225, 255);
                //     SDL_RenderRect(state->sdl_renderer, &frect);
                // }

            }

#ifndef ETHER_DEBUG_COLLISION_MOUSE
#ifndef ETHER_DEBUG_COLLISION_SMOOTH
#ifdef ETHER_DEBUG_COLLISION_STEP
            if ((hit.t < 1) && state->smth)
#else
            if (hit.t < 1)
#endif
            {
                // printf("hit %d ", hit_block);
                #define HIT_STRENGTH 1
                if (state->blocks->counts[hit_block] > HIT_STRENGTH)
                {
                    state->blocks->counts[hit_block] -= HIT_STRENGTH;
                    state->stats->money += HIT_STRENGTH;
                }
                else
                {
                    state->blocks->counts[hit_block] = 0;
                }
                // printf("%u\n", --(state->blocks->counts[hit_block]));
            }
#endif
#endif

            float delta_x = vel.x * hit.t;
            float delta_y = vel.y * hit.t;
            pos.x += delta_x;
            pos.y += delta_y;
            rect.x = pos.x;
            rect.y = pos.y;
            vel.x -= delta_x;
            vel.y -= delta_y;
            int hit_side_hori = (hit.side == ETHER_INTERSECTION_SIDE_LEFT || hit.side == ETHER_INTERSECTION_SIDE_RIGHT);
            int hit_side_vert = (hit.side == ETHER_INTERSECTION_SIDE_TOP || hit.side == ETHER_INTERSECTION_SIDE_BOTTOM);
            int mult_x = (hit_side_hori) ? -1 : 1;
            int mult_y = (hit_side_vert) ? -1 : 1;
            vel.x *= mult_x;
            vel.y *= mult_y;
            vel_sign_x *= mult_x;
            vel_sign_y *= mult_y;

#ifndef ETHER_DEBUG_COLLISION_MOUSE
#ifdef ETHER_DEBUG_COLLISION_SMOOTH
#ifdef ETHER_DEBUG_COLLISION_STEP
            if (j == 0 && state->smth)
#else
            if (j == 0)
#endif
            {
                state->entities->transforms[i].pos.x += SIGN2(delta_x);
                state->entities->transforms[i].pos.y += SIGN2(delta_y);
                if (ABS(delta_x) == 1)
                    state->entities->transforms[i].vel.x *= mult_x;
                if (ABS(delta_y) == 1)
                    state->entities->transforms[i].vel.y *= mult_y;
                
#ifdef ETHER_DEBUG_COLLISION_STEP
                if (hit.t < 1 && (ABS(delta_x) == 1 || ABS(delta_y) == 1))
#else
                if (hit.t < 1 && (ABS(delta_x) == 1 || ABS(delta_y) == 1))
#endif
                {
                    #define HIT_STRENGTH 1
                    if (state->blocks->counts[hit_block] > HIT_STRENGTH)
                    {
                        state->blocks->counts[hit_block] -= HIT_STRENGTH;
                        state->stats->money += HIT_STRENGTH;
                    }
                    else
                    {
                        state->blocks->counts[hit_block] = 0;
                    }
                }
            }
#endif
#endif

#ifdef ETHER_DEBUG_COLLISION_LINES
            frect = FRECT(rect);
            SDL_SetRenderDrawColor(state->sdl_renderer, 0, 0, 255, 255);
            SDL_RenderRect(state->sdl_renderer, &frect);
#endif

        }

#ifndef ETHER_DEBUG_COLLISION_MOUSE
#ifndef ETHER_DEBUG_COLLISION_SMOOTH
#ifdef ETHER_DEBUG_COLLISION_STEP
        if (state->smth)
#endif
        {
            state->entities->transforms[i].pos = pos;
            state->entities->transforms[i].vel.x *= vel_sign_x;
            state->entities->transforms[i].vel.y *= vel_sign_y;
        }
#endif
#else
        state->entities->transforms[i].pos.x = state->mouse_x * 2;
        state->entities->transforms[i].pos.y = state->mouse_y * 2;
#endif
    }
}
