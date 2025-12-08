// graphics.c

#include "graphics.h"
#include "game_data.h"

// Create sprite with a tracked VRAM number, assign tile, create and return GameSprite object
GameSprite create_sprite(uint8_t tile_index)
{
    GameSprite new_sprite;
    new_sprite.sprite_index = game.graphics.next_sprite_id;
    new_sprite.tile_index = tile_index;
    set_sprite_tile(game.graphics.next_sprite_id, tile_index);
    game.graphics.next_sprite_id++;

    return new_sprite;
}

// Plot a point at position in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y)
{
    plot_point(FROM_FIXED(x), FROM_FIXED(y));
}

void set_game_background(unsigned char *background, unsigned char *tiles)
{
    game.graphics.active_background_tilemap = background;
    game.graphics.active_background_tiledata = tiles;

    set_bkg_tiles(0, 0, 20, 18, game.graphics.active_background_tilemap);
    set_bkg_data(0, 255, game.graphics.active_background_tiledata);
}

void set_sprite_sheet(unsigned char *sprite_sheet)
{
    game.graphics.active_sprite_sheet = sprite_sheet;
    set_sprite_data(0, 16, game.graphics.active_sprite_sheet);
}

/* End of graphics.c */