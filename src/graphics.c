// graphics.c

#include "types.h"
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
void plot_point_fixed(fixed_t x, fixed_t y)
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

void set_active_basic_font(unsigned char *font, uint16_t size)
{
    if(!game.graphics.active_font){

        game.graphics.basic_font_vram_start_location = game.graphics.next_background_tile_slot;
        load_background_tiles(font, size);
        game.graphics.active_font = font;

    }

}

void print_text(char* str, uint8_t cursor_start_x, uint8_t cursor_start_y)
{

    uint16_t index = 0;

    uint16_t cursor_x = cursor_start_x;
    uint16_t cursor_y = cursor_start_y;

    while(str[index]!='\0'){

        // Auto new line if cursor reached edge of screen
        if (cursor_x == BACKGROUND_WIDTH_TILES-2){

            // draw a dash if previous index was NOT a space (as this is continuation of a word)
            if ( index > 0 && str[index] != ' ' && str[index-1] != ' ' ){

                uint8_t tile_offset = get_font_tile_index('-');

                uint16_t letter_vram_location = 
                    game.graphics.basic_font_vram_start_location + tile_offset;
                
                uint8_t *cursor_address = platform_get_bkg_xy_addr(cursor_x, cursor_y);
                platform_set_vram_byte(cursor_address, letter_vram_location);

            }

            cursor_y++;
            cursor_x = cursor_start_x;

            continue;
        }

        // New line
        if ((uint8_t)str[index] == '\n'){

            cursor_y++;
            cursor_x = cursor_start_x;
            index++;
            continue;

        }

        // We need to get the tile index based on the tileset rather than ascii
        // Returns the equivalent location in the tileset of ascii value
        uint8_t tile_offset = get_font_tile_index(str[index]);

        uint16_t letter_vram_location = 
            game.graphics.basic_font_vram_start_location + tile_offset;
        
        uint8_t *cursor_address = platform_get_bkg_xy_addr(cursor_x, cursor_y);
        platform_set_vram_byte(cursor_address, letter_vram_location);

        cursor_x++;

        index++;
    }
}

uint8_t get_font_tile_index(char ascii_location) {

    // Handle Uppercase (A-Z)
    if (ascii_location >= 'A' && ascii_location <= 'Z') {
        return (ascii_location - 'A'); // Returns 0-25
    }

    // Handle Lowercase (a-z)
    if (ascii_location >= 'a' && ascii_location <= 'z') {
        return (ascii_location - 'a'); // Returns 0-25
    }
    
    // Handle Numbers (0-9)
    if (ascii_location >= '0' && ascii_location <= '9') {
        return (ascii_location - '0') + 26; // Returns 26-35
    }

    // Handle specific symbols
    if (ascii_location == ' ') return 36;
    if (ascii_location == '-') return 37;
    if (ascii_location == ',') return 38;
    if (ascii_location == '.') return 39;
    if (ascii_location == '!') return 40;
    if (ascii_location == '?') return 41;
    
    return 0; // Default to space or 'A'
    
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