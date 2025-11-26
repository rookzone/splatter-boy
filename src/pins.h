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

// Build array of pin positions from the background
void instantiate_pins_from_background(Pin *pins, uint8_t count);

// Pass in the tile number from the background map and this will return the pixel coord X
uint8_t get_x_coord_from_tile_number(uint16_t tile_number);

// Pass in the tile number from the background map and this will return the pixel coord Y
uint8_t get_y_coord_from_tile_number(uint16_t tile_number);


#endif // PINS_H
