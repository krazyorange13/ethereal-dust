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
    bool *keys = SDL_GetKeyboardState(NULL);
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

void ETHER_update_entities_and_blocks(ETHER_state *state)
{
    SDL_SetRenderDrawColor(state->sdl_renderer, 25, 25, 25, 255);
    SDL_RenderClear(state->sdl_renderer);

    // SDL_SetRenderDrawColor(state->sdl_renderer, 150, 50, 50, 255);
    // SDL_RenderLine(state->sdl_renderer, ETHER_WORLD_WIDTH / 2, 0, ETHER_WORLD_WIDTH / 2, ETHER_WORLD_HEIGHT);
    // SDL_RenderLine(state->sdl_renderer, 0, ETHER_WORLD_HEIGHT / 2, ETHER_WORLD_WIDTH, ETHER_WORLD_HEIGHT / 2);

    // for (ETHER_entity_id_t i = 0; i < state->entities->len; i++)
    // {
        // if (pos->x + vel->x < 0)
        // {
        //     pos->x = -vel->x - pos->x;
        //     vel->x = -vel->x;
        // }
        // else if (pos->x + vel->x > ETHER_WORLD_WIDTH - ETHER_ENTITY_SIZE)
        // {
        //     pos->x = (ETHER_WORLD_WIDTH - ETHER_ENTITY_SIZE) - (vel->x - ((ETHER_WORLD_WIDTH - ETHER_ENTITY_SIZE) - pos->x));
        //     vel->x = -vel->x;
        // }
        // else
        // {
        //     pos->x += vel->x;
        // }

        // if (pos->y + vel->y < 0)
        // {
        //     pos->y = -vel->y - pos->y;
        //     vel->y = -vel->y;
        // }
        // else if (pos->y + vel->y > ETHER_WORLD_HEIGHT - ETHER_ENTITY_SIZE)
        // {
        //     pos->y = (ETHER_WORLD_HEIGHT - ETHER_ENTITY_SIZE) - (vel->y - ((ETHER_WORLD_HEIGHT - ETHER_ENTITY_SIZE) - pos->y));
        //     vel->y = -vel->y;
        // }
        // else
        // {
        //     pos->y += vel->y;
        // }

        // SWEPT AABB DISCRETE COLLISION HANDLING

        // using u16 here poses the risk that there will be overflow.
        // either use s16 (which could work quite well, the worlds are NOT that big lol)
        // or add a buffer so WORLD_TOP = entity velocity max or smth, more efficient

        // this collision handling stuff needs to be extremely robust, and handle
        // hitting multiple blocks in one frame

        /*

        ETHER_vec_u16 *pos = &state->entities->transforms[i].pos;
        ETHER_vec_s16 *vel = &state->entities->transforms[i].vel;

        ETHER_rect_u16 rect;
        rect.x = pos->x;
        rect.y = pos->y;
        rect.w = pos->x + ETHER_ENTITY_SIZE;
        rect.h = pos->y + ETHER_ENTITY_SIZE;

        ETHER_rect_u16 rect_moved = rect;
        rect_moved.x += vel->x;
        rect_moved.y += vel->y;

        ETHER_rect_u16 rect_swept;
        rect_swept.x = MIN(rect.x, rect_moved.x);
        rect_swept.y = MIN(rect.y, rect_moved.y);
        rect_swept.w = MAX(rect.x + rect.w, rect_moved.x + rect_moved.w) - rect.x;
        rect_swept.h = MAX(rect.y + rect.h, rect_moved.y + rect_moved.h) - rect.y;

        for (ETHER_block_id_t j = 0; j < state->blocks->len; j++)
        {
            ETHER_rect_u16 block_rect = state->blocks->rects[i];
            if (!ETHER_rects_collide_u16(rect_swept, block_rect)) continue;
            // we have a (potential) hit!
            
        }

        */

       // MARK: discrete k

/*
        float x, y;

        SDL_GetMouseState(&x, &y);

       state->entities->transforms[i].pos.x = FLOOR(x);
       state->entities->transforms[i].pos.y = FLOOR(y);
    //    state->entities->transforms[i].pos.x += state->entities->transforms[i].vel.x;
    //    state->entities->transforms[i].pos.y += state->entities->transforms[i].vel.y;

        ETHER_rect_u16 rect_curr;
        rect_curr.x = state->entities->transforms[i].pos.x;
        rect_curr.y = state->entities->transforms[i].pos.y;
        rect_curr.w = ETHER_ENTITY_SIZE;
        rect_curr.h = ETHER_ENTITY_SIZE;
        ETHER_rect_u16 rect_othr = state->blocks->rects[0];

        ETHER_rect_s16 md = ETHER_rects_minkowski_difference(rect_othr, rect_curr);
        if (md.x < 0 && md.y < 0 && md.x + md.w > 0 && md.y + md.h > 0)
        {
            printf("\x1b[2m[tick %d]\x1b[0m minkowski hit\n", state->tick);
            ETHER_vec_s16 pent = ETHER_rect_closest_point_on_bounds_to_point(md, (ETHER_vec_s16) {0, 0});//ETHER_rects_penetration_vector(rect_curr, rect_othr);
            state->entities->transforms[i].pos.x += pent.x;
            state->entities->transforms[i].pos.y += pent.y;
        }
*/

       // MARK: swept k

/*
        ETHER_rect_u16 rect_curr;
        rect_curr.x = state->entities->transforms[i].pos.x;
        rect_curr.y = state->entities->transforms[i].pos.y;
        rect_curr.w = ETHER_ENTITY_SIZE;
        rect_curr.h = ETHER_ENTITY_SIZE;
        ETHER_rect_u16 rect_othr = state->blocks->rects[0];

        ETHER_rect_u16 md = ETHER_rects_minkowski_difference(rect_othr, rect_curr);
        if (ETHER_vec_in_rect_u16((ETHER_vec_u16) {0, 0}, md))
        {
            // normal discrete collision detection / separation code
        }
        else
        {
            // for relative collisions, but doesn't rlly do much because
            // we are only colliding against static blocks
            // var relativeMotion:Vector = (boxA.velocity - boxB.velocity) * dt;
            ETHER_vec_s16 relvel = state->entities->transforms[i].vel;


        }
*/

/*
var md:AABB = boxB.minkowskiDifference(boxA);
if (md.min.x <= 0 &&
    md.max.x >= 0 &&
    md.min.y <= 0 &&
    md.max.y >= 0)
{
    // normal discrete collision detection / separation code
}
else
{
    // calculate the relative motion between the two boxes
    var relativeMotion:Vector = (boxA.velocity - boxB.velocity) * dt;

    // ray-cast the relativeMotion vector against the Minkowski AABB
    var h:Float = md.getRayIntersectionFraction(Vector.zero, relativeMotion);

    // check to see if a collision will happen this frame
    // getRayIntersectionFraction returns Math.POSITIVE_INFINITY if there is no intersection
    if(h < Math.POSITIVE_INFINITY)
    {
        // yup, there WILL be a collision this frame
        // move the boxes appropriately
        boxA.center += boxA.velocity * dt * h;
        boxB.center += boxB.velocity * dt * h;

        // zero the normal component of the velocity
        // (project the velocity onto the tangent of the relative velocities
        //  and only keep the projected component, tossing the normal component)
        var tangent:Vector = relativeMotion.normalized.tangent;
        boxA.velocity = Vector.dotProduct(boxA.velocity, tangent) * tangent;
        boxB.velocity = Vector.dotProduct(boxB.velocity, tangent) * tangent;
    }
    else
    {
        // no intersection, move it along
        boxA.center += boxA.velocity * dt;
        boxB.center += boxB.velocity * dt;
    }
}
*/
        // MARK: swept kr

/*
        ETHER_vec_u16 pos = state->entities->transforms[i].pos;
        ETHER_vec_s16 vel = state->entities->transforms[i].vel;
        ETHER_rect_u16 rect;
        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = ETHER_ENTITY_SIZE;
        rect.h = ETHER_ENTITY_SIZE;

        ETHER_rect_s16 supersweep =
        {
            pos.x - ABS(vel.x),
            pos.y - ABS(vel.y),
            ETHER_ENTITY_SIZE + ABS(vel.x) * 2,
            ETHER_ENTITY_SIZE + ABS(vel.y) * 2
        };
        ETHER_block_id_t supersweep_blocks[state->blocks->len];
        ETHER_block_id_t supersweep_blocks_len = 0;
        for (ETHER_block_id_t j = 0; j < state->blocks->len; j++)
            if (ETHER_rects_collide_s16u16(supersweep, state->blocks->rects[j]))
                supersweep_blocks[supersweep_blocks_len++] = j;

        SDL_FRect sdl_supersweep = {supersweep.x, supersweep.y, supersweep.w, supersweep.h};
        SDL_SetRenderDrawColor(state->sdl_renderer, 175, 75, 75, 255);
        SDL_RenderRect(state->sdl_renderer, &sdl_supersweep);

        // printf("supersweep");
        // for (ETHER_block_id_t j = 0; j < supersweep_blocks_len; j++)
        //     printf(" %d", supersweep_blocks[j]);
        
        printf("hi im entity %d\n", i);

        for (uint16_t j = 0; vel.x != 0 && vel.y != 0; j++)
        {
            printf("\titer %d\n", j);

            ETHER_rect_s16 sweep;
            sweep.x = MIN(pos.x, pos.x + vel.x);
            sweep.y = MIN(pos.y, pos.y + vel.y);
            sweep.w = MAX(pos.x, pos.x + vel.x) + ETHER_ENTITY_SIZE - sweep.x;
            sweep.h = MAX(pos.y, pos.y + vel.y) + ETHER_ENTITY_SIZE - sweep.y;
            ETHER_block_id_t sweep_blocks[supersweep_blocks_len];
            ETHER_block_id_t sweep_blocks_len = 0;
            for (ETHER_block_id_t k = 0; k < supersweep_blocks_len; k++)
                if (ETHER_rects_collide_s16u16(sweep, state->blocks->rects[supersweep_blocks[k]]))
                    sweep_blocks[sweep_blocks_len++] = supersweep_blocks[k];
            
            // hit 'em with the RAYCAST!
            // but for now, just debugging...

            SDL_FRect sdl_sweep = {sweep.x, sweep.y, sweep.w, sweep.h};
            SDL_SetRenderDrawColor(state->sdl_renderer, 225, 25, 25, 255);
            SDL_RenderRect(state->sdl_renderer, &sdl_sweep);

            // printf("supersweep %d %d %d %d; sweep %d %d %d %d\n",
            //     supersweep.x, supersweep.y, supersweep.w, supersweep.h,
            //     sweep.x, sweep.y, sweep.w, sweep.h
            // );

            // printf("; sweep (%d)", j);
            // for (ETHER_block_id_t k = 0; k < sweep_blocks_len; k++)
                // printf(" %d", sweep_blocks[k]);

            // if (sweep_blocks_len == 0)
            // {
            //     state->entities->transforms[i].pos.x += vel.x;
            //     state->entities->transforms[i].pos.y += vel.y;
            // }

            // TODO: raycast against all blocks

            ETHER_bool should_move = 1;

            for (ETHER_block_id_t k = 0; k < sweep_blocks_len; k++)
            {
                printf("\t\tagainst block %d\n", k);
                // printf("i %d j %d k %d\n", i, j, k);
                ETHER_rect_s16 md = ETHER_rects_minkowski_difference(state->blocks->rects[sweep_blocks[k]], rect);
                ETHER_raycast_side_t side;
                float intersection = ETHER_rect_ray_intersection(md, (ETHER_vec_s16) {0, 0}, vel, &side);
                // printf("intersection %d %f\n", i, intersection);
                if (intersection < INFINITY)
                {
                    printf("\t\tagainst block %d hit\n", k);
                    should_move = 0;
                    pos.x += vel.x * intersection;
                    pos.y += vel.y * intersection;
                    state->entities->transforms[i].pos = pos;
                    vel.x -= vel.x * intersection;
                    vel.y -= vel.y * intersection;
                    rect.x = pos.x;
                    rect.y = pos.y;
                    break;
                    // // flip here, (depends on raycast)
                    // if (side == ETHER_RAYCAST_SIDE_TOP || side == ETHER_RAYCAST_SIDE_BOTTOM)
                    //     vel.y *= -1;
                    // else if (side == ETHER_RAYCAST_SIDE_RIGHT || side == ETHER_RAYCAST_SIDE_LEFT)
                    //     vel.x *= -1;
                    // break;
                }
            }

            vel.x = 0;
            vel.y = 0;

            printf("\tfree move %d\n", should_move);

            if (should_move)
            {
                pos.x += vel.x;
                pos.y += vel.y;
                state->entities->transforms[i].pos = pos;
                break;
            }
        }

        // printf(";\n");
*/

    // }

    // MARK: testing

    // #define LINE(L) L.x1, L.y1, L.x2, L.y2
    // #define RECT(R) R.x, R.y, R.w, R.h

    // #define RENDERLINE(L) SDL_RenderLine(state->sdl_renderer, L.x1, L.y1, L.x2, L.y2);
    // #define RENDERRECT(R) frect = (SDL_FRect) {R.x, R.y, R.w, R.h}; SDL_RenderRect(state->sdl_renderer, &frect);
    // SDL_FRect frect;

    /* LINE TESTS PASSED

    ETHER_line l1 = {0, 0, 100, 100},
               l2 = {100, 0, 0, 100},
               l3 = {50, 0, 50, 100},
               l4 = {40, 20, 40, 80},
               l5 = {60, 20, 60, 80},
               l6 = {40, 20, 60, 20},
               l7 = {40, 80, 60, 80},
               l8 = {0, 50, 100, 50};

    SDL_SetRenderDrawColor(state->sdl_renderer, 200, 20, 20, 255);
    SDL_RenderLine(state->sdl_renderer, LINE(l1));
    SDL_RenderLine(state->sdl_renderer, LINE(l2));
    SDL_RenderLine(state->sdl_renderer, LINE(l3));
    SDL_RenderLine(state->sdl_renderer, LINE(l4));
    SDL_RenderLine(state->sdl_renderer, LINE(l5));
    SDL_RenderLine(state->sdl_renderer, LINE(l6));
    SDL_RenderLine(state->sdl_renderer, LINE(l7));
    SDL_RenderLine(state->sdl_renderer, LINE(l8));

    #define DOESHIT(L1, L2) ETHER_bool L1##L2 = (ETHER_line_intersection(L1, L2) < INFINITY); \
                            printf("DOESHIT " #L1 " " #L2 " IS %d\n", L1##L2);

    if (!state->smth) return;

    DOESHIT(l3, l1)
    DOESHIT(l3, l2)
    DOESHIT(l3, l3)
    DOESHIT(l3, l4)
    DOESHIT(l3, l5)
    DOESHIT(l3, l6)
    DOESHIT(l3, l7)
    DOESHIT(l3, l8)

    printf("\n\n");

    */

/*

    ETHER_rect r1 = {100, 100, 100, 100};
    ETHER_line l1 = {150, 150, 175, 250};

    SDL_SetRenderDrawColor(state->sdl_renderer, 200, 20, 20, 255);
    // SDL_RenderLine(state->sdl_renderer, LINE(l1));
    // SDL_RenderRect(state->sdl_renderer, )
    RENDERLINE(l1)
    RENDERRECT(r1)

    #define DOESHIT(R, L) ETHER_intersection_data hit_##R##L = ETHER_rect_line_intersection(R, L); \
                          printf("DOESHIT " #R " " #L " IS %d %d\n", hit_##R##L.t < INFINITY, hit_##R##L.side);

    if (state->smth)
    {
        DOESHIT(r1, l1);
    }

*/

    // return;

    // MARK: swept kr mk2

    SDL_FRect frect;

    for (ETHER_entity_id_t i = 0; i < state->entities->len; i++)
    {
        // if (state->smth)
        //     printf("hi im entity %d\n", i);

        ETHER_vec pos = state->entities->transforms[i].pos;
        ETHER_vec vel = state->entities->transforms[i].vel;
        ETHER_rect rect = {pos.x, pos.y, ETHER_ENTITY_SIZE, ETHER_ENTITY_SIZE};

        frect = FRECT(rect);
        SDL_SetRenderDrawColor(state->sdl_renderer, 0, 0, 255, 255);
        SDL_RenderRect(state->sdl_renderer, &frect);
        
        ETHER_rect supersweep = ETHER_get_supersweep(rect, vel);
        ETHER_block_id_t supersweep_blocks[state->blocks->len];
        ETHER_block_id_t supersweep_blocks_len = 0;
        for (ETHER_block_id_t k = 0; k < state->blocks->len; k++)
            if (ETHER_rects_collide(supersweep, state->blocks->rects[k]))
                supersweep_blocks[supersweep_blocks_len++] = k;

        frect = FRECT(supersweep);
        SDL_SetRenderDrawColor(state->sdl_renderer, 255, 0, 255, 255);
        SDL_RenderRect(state->sdl_renderer, &frect);
        
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
            
            frect = FRECT(sweep);
            SDL_SetRenderDrawColor(state->sdl_renderer, 255, 0, 255, 255);
            SDL_RenderRect(state->sdl_renderer, &frect);
            
            ETHER_intersection_data hit = {1, ETHER_INTERSECTION_SIDE_NONE};
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
                if (_hit.t < hit.t) hit = _hit;

                // if (_hit.t < INFINITY || ETHER_vec_in_rect((ETHER_vec) {0, 0}, md))
                // {

                //     if (state->smth)
                //         printf("\t\tblock %d hit\n", sweep_blocks[k]);
                //     frect = FRECT(state->blocks->rects[sweep_blocks[k]]);
                //     SDL_SetRenderDrawColor(state->sdl_renderer, 225, 255, 225, 255);
                //     SDL_RenderRect(state->sdl_renderer, &frect);
                // }
            }

            // TODO: there is likely a large rounding error here. figure out a way
            // to circumvent it: maybe just use float vecs? but all the functions
            // will be screwed then. dammit. at some point we will need floats.
            // and really, i guess all of the operations should/could whatever be actual
            // floats, for proper minkowski and raycasting and stuff.
            // sweeps could just be rounded floats but whatever. UGHHH more 
            // functions and boilerplate and whatever, but I want to see if 
            // things work without the floats first

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

            frect = FRECT(rect);
            SDL_SetRenderDrawColor(state->sdl_renderer, 0, 0, 255, 255);
            SDL_RenderRect(state->sdl_renderer, &frect);
        }

        if (state->smth)
        {
            state->entities->transforms[i].pos = pos;
            state->entities->transforms[i].vel.x *= vel_sign_x;
            state->entities->transforms[i].vel.y *= vel_sign_y;
        }
    }
}
