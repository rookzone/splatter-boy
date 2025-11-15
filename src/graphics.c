// graphics.c

#include <gb/gb.h>
#include "graphics.h"

uint8_t next_sprite_id = 0;

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

// Moves sprite using fixed-point coordinates (e.g., from Ball physics)
// This function internally handles the conversion (FROM_FIXED)
void move_sprite_fixed(GameSprite* game_sprite, fixed_n x, fixed_n y)
{
    move_sprite(game_sprite->sprite_index, FROM_FIXED(x), FROM_FIXED(y));
}

// Moves sprite using integer pixel coordinates (e.g., for Wall or initialization)
void move_sprite_int(GameSprite* game_sprite, uint8_t x, uint8_t y)
{
    move_sprite(game_sprite->sprite_index, x, y);
}

// Plot a point in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y)
{
    plot_point(FROM_FIXED(x), FROM_FIXED(y));
}