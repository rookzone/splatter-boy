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






uint8_t pin_tile_location[20][18];

unsigned char pin_tile_value = 0;

uint16_t background_array_length = sizeof(game_background);

void get_pin_coords_from_bkg(void)
{
    /*
    **Required values for algo**
    game_background
    game_background_tiles
    pin_tile_value
    pin_tile_location[][]
    background_array_length
    
    if game_background[i] == pin_tile_value..

        tile_loc_x = get_x_coord_from_tile_number(pin_tile_value) = 
        tile_loc_y = get_y_coord_from_tile_number(pin_tile_value)
    */

    // pin_tile_location[1][1] = tile_loc_x tile_loc_y //pseudo...
   
}

uint8_t get_x_coord_from_tile_number(uint8_t tile_number)
{
    // return floor(tile_number / ROW_LENGTH);
}

uint8_t get_y_coord_from_tile_number(uint8_t tile_number)
{
    // return tile_number % ROW_LENGTH;
}
