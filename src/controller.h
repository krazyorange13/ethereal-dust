#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "state.h"

void ETHER_update(ETHER_state *state);
void ETHER_update_events(ETHER_state *state);
void ETHER_update_event(ETHER_state *state, SDL_Event *event);
void ETHER_update_entities_and_blocks(ETHER_state *state);

#endif