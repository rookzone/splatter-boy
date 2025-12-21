/**
 * @file graphics.h
 * @brief Handles drawing of background, window or sprite tiles on screen.
 * * @details Contains useful macros for handling tile grids and pixels.
 * Functions to load tiles, backgrounds, and sprites. The GameSprite struct is used.
 * This contains the tile_index and sprite_index, these are indices that reference the ID in memory.
 * Memory is managed in here, so the logic can just ask for something to be drawn without worrying about IDs and memory.
 * The GameSprite object reference that is returned can be used to call graphics functions.
 * Passing around structs is slow, so try and keep that stuff out of any update loops.
 * * @defgroup Graphics
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "types.h"
#include "platform/gb/platform.h"

// Byte that represents the PIN. Used for collision checks 
#define PIN_TILE_ID 0x02

// Convert Pixel coordinate to Grid coordinate (Divide by 8)
#define PIXEL_TO_GRID(x) ((x) >> 3)

// Convert Grid coordinate to Pixel coordinate (Multiply by 8)
#define GRID_TO_PIXEL(x) ((x) << 3)

// Get tilemap Array Index from Col/Row (Row * Width + Col)
#define GET_TILE_INDEX(col, row) ((((uint16_t)(row) << 4) + ((uint16_t)(row) << 2)) + (col))

// === SPRITE HANDLING ===

// Allocates a sprite at sprite_index with a tile at tile_index
GameSprite create_sprite(uint8_t tile_index);

// === BACKGROUNDS ===

// Loads background tiles and map into memory
void set_game_background(unsigned char *background, unsigned char *tiles);

unsigned char* get_game_background_tilemap(void);

void load_background_tiles(unsigned char *tiles, uint16_t count);

// Load in a sprite sheet
void set_sprite_sheet(unsigned char *sprite_sheet);

// === DRAWING ===

// Fast sprite draw using GBDK in move_sprite routine. Pass pointer to GameSprite
// Offsets are added here because Game Boy sprites are rendered at (x+8, y+16)
#define DRAW_SPRITE(obj_ptr, x, y) \
    platform_move_sprite((obj_ptr)->sprite_index, x + 8, y + 16)

// Plot a point at position in pixel space (int)
void plot_point_fixed(fixed_t x, fixed_t y);

void gr_hide_all_sprites(void);

// === ANIMATIONS ===



// === FONT AND TEXT ===

// Returns the tile index of the font as it does not map to ascii
uint8_t get_font_tile_index(char c);

void print_text(char* str, uint8_t cursor_start_x, uint8_t cursor_start_y);

void text_clear_screen(void);

void set_active_basic_font(unsigned char *font, uint16_t size);

#endif // GRAPHICS_H_