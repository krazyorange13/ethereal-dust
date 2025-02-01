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
}

void ETHER_update_entities_and_blocks(ETHER_state *state)
{
    for (ETHER_entity_id_t i = 0; i < state->entities->len; i++)
    {
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
        if (md.x <= 0 && md.y <= 0 && md.x + md.w >= 0 && md.y + md.h >= 0)
        {
            printf("%d\n", state->tick);
            ETHER_vec_s16 pent = ETHER_rect_closest_point_on_bounds_to_point(md, (ETHER_vec_s16) {0, 0});//ETHER_rects_penetration_vector(rect_curr, rect_othr);
            state->entities->transforms[i].pos.x += pent.x;
            state->entities->transforms[i].pos.y += pent.y;
        }

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
    }
}