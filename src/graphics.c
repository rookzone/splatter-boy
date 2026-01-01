// graphics.c

#include "types.h"
#include "graphics.h"
#include "game_state.h"
#include <stdio.h>
#include "tiles/fillBarTiles.h"

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

void set_game_background(unsigned char *background, unsigned char *tiles, uint16_t tileset_size)
{
    game.graphics.active_background_tilemap = background;
    game.graphics.active_background_tileset = tiles;
    game.graphics.active_background_tileset_size = tileset_size;
    game.graphics.background_vram_start_location = game.graphics.next_background_tile_slot;

    platform_set_bkg_data(
        game.graphics.background_vram_start_location,
        game.graphics.active_background_tileset_size,
        game.graphics.active_background_tileset
    );

    platform_set_bkg_tiles(0, 0, BACKGROUND_WIDTH_TILES, BACKGROUND_HEIGHT_TILES, game.graphics.active_background_tilemap);
    
    game.graphics.next_background_tile_slot += game.graphics.active_background_tileset_size;
}

void reload_active_background(void)
{
    platform_set_bkg_tiles(0, 0, BACKGROUND_WIDTH_TILES, BACKGROUND_HEIGHT_TILES, game.graphics.active_background_tilemap);
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


void text_clear_screen(void)
{
    for (uint8_t i = 1; i <= DEVICE_SCREEN_HEIGHT; i++){
        print_text("                    ",1,i);
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

    switch (ascii_location){

        case ' ':
            return 36;
        break;

        case '-':
            return 37;
        break;

        case ',':
            return 38;
        break;

        case '.':
            return 39;
        break;

        case '!':
            return 40;
        break;

        case '?':
            return 41;
        break;

        case '>':
            return 42;
        break;

    }
    
    return 36; // Default to ' '
}


void set_sprite_sheet(unsigned char *sprite_sheet)
{
    game.graphics.active_sprite_sheet = sprite_sheet;
    platform_set_sprite_data(0, 16, game.graphics.active_sprite_sheet);
}

void hide_all_sprites(void)
{ 
// Iterate through all 40 hardware sprites
    for (uint8_t i = 0; i < NUM_HW_SPRITES; i++) {
        // This removes any sprites from the screen, they can be reallocated.
        platform_move_sprite(i, 0, 0); 
    }
}

// === GUI ===

// Load into vram and draw on window layer
void init_fill_bar(uint8_t x, uint8_t y)
{
    // Load tiles in
    //load_background_tiles(bar,BAR_LENGTH_TILES);
    // Draw on window at xy
    
}

//char* bar_tile_graphics[] = {'0','1','2','3','4'};

unsigned char bar_tile_graphics[] =
{
  0xFF,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,
  0x00,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0xFF,
  0xFF,0xFF,0x00,0xC0,0x00,0xC0,0x00,0xC0,
  0x00,0xC0,0x00,0xC0,0x00,0xC0,0xFF,0xFF,
  0xFF,0xFF,0x00,0xF0,0x00,0xF0,0x00,0xF0,
  0x00,0xF0,0x00,0xF0,0x00,0xF0,0xFF,0xFF,
  0xFF,0xFF,0x00,0xFC,0x00,0xFC,0x00,0xFC,
  0x00,0xFC,0x00,0xFC,0x00,0xFC,0xFF,0xFF,
  0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF
};
// How do I address the above??

void update_fill_bar(uint8_t fill_value)
{

    gotoxy(1,1);
    // Create actual fill bar on screen and in VRAM
    //if (!bar_tiles)
       // init_fill_bar(POWER_BAR_X, POWER_BAR_Y);

    // Calculate number of increments or steps the bar is to be filled by
    uint8_t fill_steps = fill_value / 10; //
    uint8_t full_tiles = fill_steps / BAR_STEPS_PER_TILE;
    uint8_t remainder = fill_steps % BAR_STEPS_PER_TILE; 


    for (uint8_t i = 0; i < full_tiles; i++) {
    
        //bar_tiles[i] = bar_tile_graphics[tile_to_draw];
        printf("%d", 0);
        //print_text(bar_tile_graphics[tile_to_draw], i, 2);
    }
        //bar_tiles[tiles_to_fill] = bar_tile_graphics[fill_steps];
       // print_text(bar_tile_graphics[fill_steps], tiles_to_fill, 2);
       if (remainder != 0)
        printf("%d", remainder);

}

/* End of graphics.c */