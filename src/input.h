#ifndef INPUT_H_
#define INPUT_H_

#include "custom_types.h"

static inline uint8_t get_key_down(uint8_t key)
{
    return(game.system.keys & key && !(game.system.previous_keys & key));
}


#endif // INPUT_H_