// pins.c
#include "pins.h"
#include <stdio.h>


void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y)
{

    pin->x = pin_x;
    pin->y = pin_y;

    *gfx_data = create_sprite(TILE_PIN);
    pin->game_sprite = gfx_data;

}

void get_pin_coords_from_bkg(void)
{
    // code
}
