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

#include <gb/drawing.h>
#include "custom_types.h"

// Byte that represents the PIN. Used for collision checks 
#define PIN_TILE_ID 0x02

// Convert Pixel coordinate to Grid coordinate (Divide by 8)
#define PIXEL_TO_GRID(x) ((x) >> 3)

// Convert Grid coordinate to Pixel coordinate (Multiply by 8)
#define GRID_TO_PIXEL(x) ((x) << 3)

// Get tilemap Array Index from Col/Row (Row * Width + Col)
#define GET_TILE_INDEX(col, row) (((uint16_t)(row) * BACKGROUND_WIDTH_TILES) + (col))

// General gameboy graphics sizes
#define TILE_WIDTH 8
#define TILE_LENGTH 8
#define TILE_HALF_WIDTH 4
#define TILE_HALF_LENGTH 4
#define SPRITE_SIZE 8
#define BACKGROUND_WIDTH_TILES 20
#define BACKGROUND_HEIGHT_TILES 18
#define BACKGROUND_WIDTH_PIXELS 160
#define BACKGROUND_HEIGHT_PIXELS 144

// Friendly name that corresponds with tile position in sprite sheet(s). 
enum { TILE_BALL = 0, TILE_WALL = 1, TILE_PIN = 2 };


// === SPRITE HANDLING ===

// Allocates a sprite at sprite_index with a tile at tile_index
// Returns GameSprite struct that stores these values, plus other useful graphics data TBC
GameSprite create_sprite(uint8_t tile_index);

// === BACKGROUNDS ===

// Loads background tiles and map into memory
void set_game_background(unsigned char *background, unsigned char *tiles);

// Load in a sprite sheet
void set_sprite_sheet(unsigned char *sprite_sheet);

// === DRAW FUNCTIONS ===

// Fast sprite draw using GBDK in move_sprite routine. Pass pointer to GameSprite
// Offsets are added here because Game Boy sprites are rendered at (x+8, y+16)
#define DRAW_SPRITE(obj_ptr, x, y) \
    move_sprite((obj_ptr)->sprite_index, x + 8, y + 16)

// Plot a point at position in pixel space (int)
void plot_point_fixed(fixed_n x, fixed_n y);

// === ANIMATIONS ===
// Future-proofing for animation functions

#endif // GRAPHICS_H_