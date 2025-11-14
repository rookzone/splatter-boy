#ifndef GRAPHICS_H
#define GRAPHICS_H

#include"physics.h"

// Graphics index

enum { TILE_BALL = 0, TILE_WALL = 1 };

// Sprite handling

extern uint8_t next_sprite_id;

struct GameSprite {
    uint8_t sprite_index;
    uint8_t tile_index;
};

// Function to assign the next available sprite ID and initialize its tile
GameSprite create_sprite(uint8_t tile_index);

// -**Movement Functions**-

// Moves the sprite using fixed decimal space values
void move_sprite_fixed(GameSprite* game_sprite, fixed_n x, fixed_n y);

// Move the sprite using integer screen values
void move_sprite_int(GameSprite* game_sprite, uint8_t x, uint8_t y);

#endif