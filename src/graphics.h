// graphics.h

#include <gb/gb.h>
#include <gb/drawing.h>
#include "customtypes.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

// Graphics indices. There should be one of these per sprite sheet.
// Friendly name that corresponds with tile position in sprite sheet(s). 
enum { TILE_BALL = 0, TILE_WALL = 1, TILE_PIN = 2 };

#define PIN_TILE_VALUE 

// === SPRITE HANDLING ===

// This is used to keep track of which sprite_index is available
// The value of this number is the number of sprites "instantiated"
extern uint16_t next_sprite_id;

// Allocates a sprite at sprite_index with a tile at tile_index.
// Returns GameSprite struct that stores these values, plus other useful graphics data TBC
GameSprite create_sprite(uint8_t tile_index);

// === BACKGROUND HANDLING ===

// Pointer to current backround array
unsigned char *game_background;
unsigned char *game_background_tiles;

void set_game_background(unsigned char *background, unsigned char *tiles);

// === DRAW FUNCTIONS ===

// Fast sprite draw using GBDK in move_sprite routine. Pass pointer to GameSprite
#define DRAW_SPRITE(obj_ptr, x, y) \
    move_sprite((obj_ptr)->sprite_index, x, y)

// === ANIMATIONS ===

// forward declaration from GBDK for clarity
void plot_point(uint8_t x, uint8_t y) OLDCALL;

// Plot a point in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y);


#endif