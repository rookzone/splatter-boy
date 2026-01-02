#ifndef INPUT_H_
#define INPUT_H_

#include "types.h"

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



#endif // INPUT_H_