// pins.h

#ifndef PINS_H
#define PINS_H

#include "customtypes.h"

// Pass an array of type Pin to initialise the pins.
void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y);

// Pass a pin to initialise without graphics (for pins created using background)
void init_background_pin(Pin* pin, uint8_t pin_x, uint8_t pin_y);

#endif // PINS_H
