// graphics.h

#include "customtypes.h"
#include <gb/drawing.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

// Graphics indices.
enum { TILE_BALL = 0, TILE_WALL = 1 };

// ##### SPRITE HANDLING #####

extern uint8_t next_sprite_id;

// Function to assign the next available sprite ID and initialize its tile
GameSprite create_sprite(uint8_t tile_index);

// ##### DRAW FUNCTIONS ######

// Moves the sprite using fixed decimal space values
void move_sprite_fixed(GameSprite* game_sprite, fixed_n x, fixed_n y);

// Move the sprite using integer screen values
void move_sprite_int(GameSprite* game_sprite, uint8_t x, uint8_t y);

/* Fast sprite draw using built in move_sprite routine, 
passed "fixed" space numbers which are bit shifted into 8bit integers for pixel rendering position */
#define DRAW_SPRITE(obj_ptr, fixed_x, fixed_y) \
    move_sprite((obj_ptr)->sprite_index, FROM_FIXED(fixed_x), FROM_FIXED(fixed_y))


// ##### ANIMATIONS #####

// forward declaration for clarity
void plot_point(uint8_t x, uint8_t y) OLDCALL;

// Plot a point in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y);


#endif