#ifndef INPUT_H
#define INPUT_H

#include "../types.h"
#include "../game_state.h"

// Module: Input
// Dependencies: core (game_state, platform)

static inline uint8_t get_key_pressed(uint8_t key)
{
    return(game.system.keys & key && !(game.system.previous_keys & key));
}

static inline uint8_t get_key_released(uint8_t key)
{
    return(game.system.previous_keys & key && !(game.system.keys & key));
}


static inline uint8_t get_key_held(uint8_t key)
{
    return(game.system.keys & key);
}

// === MODULE WRAPPER ===
void module_input_init(void);
void module_input_update(void);
void module_input_shutdown(void);



#endif // INPUT_H
