// graphics.c

#include "graphics.h"
#include "game_data.h"
#include <stdio.h>

// Create sprite with a tracked VRAM number, assign tile, create and return GameSprite object
GameSprite create_sprite(uint8_t tile_index)
{
    GameSprite new_sprite;
    new_sprite.sprite_index = game.graphics.next_sprite_slot;
    new_sprite.tile_index = tile_index;
    platform_set_sprite_tile(game.graphics.next_sprite_slot, tile_index);
    game.graphics.next_sprite_slot++;

    return new_sprite;
}

// Plot a point at position in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y)
{
    platform_plot_point(FROM_FIXED(x), FROM_FIXED(y));
}

void set_game_background(unsigned char *background, unsigned char *tiles)
{
    game.graphics.active_background_tilemap = background;
    game.graphics.active_background_tileset = tiles;

    platform_set_bkg_data(game.graphics.next_background_tile_slot, BACKGROUND_TILESET_SIZE, game.graphics.active_background_tileset);

    platform_set_bkg_tiles(0, 0, BACKGROUND_WIDTH_TILES, BACKGROUND_HEIGHT_TILES, game.graphics.active_background_tilemap);
    
    game.graphics.next_background_tile_slot += BACKGROUND_TILESET_SIZE;
}

void load_background_tiles(unsigned char *tiles, uint16_t count)
{
    // Load tiles into vram at next available vram slot, size count, array tiles
    platform_set_bkg_data(game.graphics.next_background_tile_slot, count, tiles);

    // Increment next available vram slot
    game.graphics.next_background_tile_slot += count;
}

unsigned char* get_game_background_tilemap(void)
{
    return game.graphics.active_background_tilemap;
}

void set_active_font_upper_case(unsigned char *font, uint16_t size)
{

    game.graphics.upper_case_font_vram_start_location = game.graphics.next_background_tile_slot;

    load_background_tiles(font, size);

    game.graphics.active_font = font;

}

void print_text(char* str)
{
    uint16_t index = 0;

    uint16_t cursor_x = 1;
    uint16_t cursor_y = 1;

    // if UPPER CASE
    while(str[index]!='\0'){

        // Grab position in alphabet of character
        uint16_t letter_position_in_alphabet = 
            ( (uint8_t)str[index] - UPPER_CASE_ASCII_OFFSET ); // upper case offset 97

        // Tile position in vram is upper case start tile + alphabet position
        uint16_t letter_vram_location = 
            game.graphics.upper_case_font_vram_start_location + 
            letter_position_in_alphabet;
        
        uint8_t *cursor_address = platform_get_bkg_xy_addr(cursor_x, cursor_y);
        platform_set_vram_byte(cursor_address,letter_vram_location);
        cursor_x++;

        index++;
    }
}


void set_sprite_sheet(unsigned char *sprite_sheet)
{
    game.graphics.active_sprite_sheet = sprite_sheet;
    platform_set_sprite_data(0, 16, game.graphics.active_sprite_sheet);
}

void gr_hide_all_sprites(void)
{
    
// Iterate through all 40 hardware sprites
    for (uint8_t i = 0; i < NUM_HW_sPRITES; i++) {
        // This removes any sprites from the screen, they can be reallocated.
        platform_move_sprite(i, 0, 0); 
    }

}

// ===  ===

/* End of graphics.c */