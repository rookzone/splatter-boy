// pins.h

#include <gb/gb.h>
#include "customtypes.h"

#ifndef PINS_H
#define PINS_H

// Pass an array of type Pin to initialise the pins. Provide
void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t x, uint8_t y);


#endif
