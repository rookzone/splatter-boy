// pins.c

#include "pins.h"
#include "graphics.h" // Needs create_sprite, TILE_PIN

void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y)
{
    pin->x = pin_x;
    pin->y = pin_y;

    *gfx_data = create_sprite(TILE_PIN);
    pin->game_sprite = gfx_data;
}

void init_background_pin(Pin* pin, uint8_t pin_x, uint8_t pin_y)
{
    pin->x = pin_x;
    pin->y = pin_y;
    pin->game_sprite = NULL; // empty gamesprite
}

// end pins.c