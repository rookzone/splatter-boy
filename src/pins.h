// pins.h

#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"
#include "customtypes.h"

#ifndef PINS_H
#define PINS_H

// Pass an array of type Pin to initialise the pins.
void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y);

// Build array of pin positions from the background
void get_pin_coords_from_bkg();


#endif
