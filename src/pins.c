// pins.c

#include "pins.h"

void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y)
{

    pin->x = pin_x;
    pin->y = pin_y;

    *gfx_data = create_sprite(TILE_PIN);
    pin->game_sprite = gfx_data;

}


/* TODO:
Create helper functions in graphics.c/h to load in background tiles
and background map
Create variables to store this data in

Figure out how tiles are represneted in the set, and the map
Then use this to figure out how to index the map and find the pin tile
add pin tiles x/y to 2d array

Update game init to load in the map using graphics, and then instantiate the pins...


*/

// Pointer to current backround array...
//unsigned char *game_background;
//unsigned char *game_background_tiles;
// Maybe the above could just be handled in graphics.c/h
// I can use a function to return a reference rather than putting the reference in global...

//uint8_t pin_tile_location[20][18];
//unsigned char pin_tile_value = 0;
//uint16_t background_array_length = sizeof(game_background);

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

uint8_t get_y_coord_from_tile_number(uint8_t tile_number)
{
    uint8_t y_tile_index = tile_number / MAP_WIDTH_TILES;
    
    return y_tile_index*TILE_WIDTH;
}

uint8_t get_x_coord_from_tile_number(uint8_t tile_number)
{
    uint8_t x_tile_index = tile_number % MAP_WIDTH_TILES;

    return x_tile_index*TILE_WIDTH;
}
