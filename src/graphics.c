// graphics.c

#include "graphics.h"

unsigned char *active_background_tilemap = NULL;
unsigned char *active_background_data = NULL;

uint16_t next_sprite_id = 0; // current sprite index

// Create sprite with a tracked VRAM number, assign tile, create and return GameSprite object
GameSprite create_sprite(uint8_t tile_index)
{
    GameSprite new_sprite;
    new_sprite.sprite_index = next_sprite_id;
    new_sprite.tile_index = tile_index;
    set_sprite_tile(next_sprite_id, tile_index);
    next_sprite_id++;

    return new_sprite;
}

// Plot a point at position in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y)
{
    plot_point(FROM_FIXED(x), FROM_FIXED(y));
}

void set_game_background(unsigned char *background, unsigned char *tiles)
{
    active_background_tilemap = background;
    active_background_data = tiles;

    set_bkg_data(0, 4, active_background_data);
    set_bkg_tiles(0, 0, 20, 18, active_background_tilemap);

}

// end graphics.c