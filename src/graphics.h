// graphics.h

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gb/drawing.h>
#include "customtypes.h"

// Friendly name that corresponds with tile position in sprite sheet(s). 
enum { TILE_BALL = 0, TILE_WALL = 1, TILE_PIN = 2 };

// === SPRITE HANDLING ===

// This is used to keep track of the most recently available sprite index in memory
extern uint16_t next_sprite_id;

// Allocates a sprite at sprite_index with a tile at tile_index
// Returns GameSprite struct that stores these values, plus other useful graphics data TBC
GameSprite create_sprite(uint8_t tile_index);

// === BACKGROUNDS ===

// The map made of 20x18 8x8 tiles
extern unsigned char *active_background_tilemap;
// The actual tile data the map uses
extern unsigned char *active_background_data;

// Loads background tiles and map into memory
void set_game_background(unsigned char *background, unsigned char *tiles);

// === DRAW FUNCTIONS ===

// Fast sprite draw using GBDK in move_sprite routine. Pass pointer to GameSprite
// Offsets are added here because Game Boy sprites are rendered at (x+8, y+16)
#define DRAW_SPRITE(obj_ptr, x, y) \
    move_sprite((obj_ptr)->sprite_index, x + 8, y + 16)

// Plot a point at position in pixel space (int)
void plot_point_fixed(fixed_n x, fixed_n y);

// === ANIMATIONS ===
// Future-proofing for animation functions

#endif // GRAPHICS_H