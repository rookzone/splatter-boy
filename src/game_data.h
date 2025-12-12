// game_data.h

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <gb/gb.h>
#include "custom_types.h"
#include "game_object.h"

// === GAME STATE SUB SYSTEMS ===

typedef struct {

    uint8_t current_state;
    uint8_t keys;
    uint8_t previous_keys;

} System;

typedef struct {
    
    unsigned char *active_background_tilemap;
    unsigned char *active_background_tile_sheet;
    unsigned char *active_sprite_sheet;
    uint16_t next_sprite_slot;
    uint16_t next_background_slot;
    uint8_t sprite_count;

} Graphics;

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;             // Total active objects
    
    // Registries (indices into pool for fast iteration by type)
    uint8_t ball_indices[MAX_BALLS];
    uint8_t ball_count;

} ObjectManager;

// === GAME STATE ===

typedef struct {

    System system;
    Graphics graphics;
    ObjectManager objects;

} GameState;


extern GameState game;

// Clean up the state ready for a new setup
void clear_game_data(GameState* game);


#endif // GAME_DATA_H_