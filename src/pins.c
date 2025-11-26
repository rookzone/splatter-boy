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

void init_background_pin(Pin* pin, uint8_t pin_x, uint8_t pin_y)
{

    pin->x = pin_x;
    pin->y = pin_y;
    pin->game_sprite = NULL; // empty gamesprite

}

// Pass pin array
void instantiate_pins_from_background(Pin *pins, uint8_t max_capacity)
{
    // The size of the pin array to prevent assigning pins out of bounds
    // Array *should* be the same size as the number of pin tiles placed on map
    uint8_t pins_created = 0;

    // Get the number of map tiles as we need to check each one
    uint16_t num_map_tiles = BACKGROUND_WIDTH_TILES * BACKGROUND_HEIGHT_TILES;

    // For each map tile check for the pin tile
    for (uint16_t i = 0; i < num_map_tiles; i++) {

        if (pins_created >= max_capacity) break;

        // if this map tile is a pin tile and space in pin array
         if(active_background_tilemap[i] == PIN_TILE_ID){

            // Get the tile x/y location of this tile
            uint8_t tile_loc_x = get_x_coord_from_tile_index(i);
            uint8_t tile_loc_y = get_y_coord_from_tile_index(i);

            // Create pin at x,y location where background tile is a pin
            init_background_pin(&pins[pins_created], tile_loc_x, tile_loc_y);

            //plot_point(tile_loc_x, tile_loc_y);
            // decriment num pins available to allocate
            pins_created++;
        }
    }
}

uint8_t get_y_coord_from_tile_index(uint16_t tile_number)
{
    uint8_t y_tile_index = tile_number / BACKGROUND_WIDTH_TILES;
    
    return y_tile_index*TILE_WIDTH;
}

uint8_t get_x_coord_from_tile_index(uint16_t tile_number)
{
    uint8_t x_tile_index = tile_number % BACKGROUND_WIDTH_TILES;

    return x_tile_index*TILE_WIDTH;
}
