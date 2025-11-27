// pins.h

#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"
#include "customtypes.h"

#ifndef PINS_H
#define PINS_H

#define PIN_TILE_ID 0x02

// Pass an array of type Pin to initialise the pins.
void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y);

// Pass a pin to initialise without graphics (for pins created using background)
void init_background_pin(Pin* pin, uint8_t pin_x, uint8_t pin_y);



#endif // PINS_H
