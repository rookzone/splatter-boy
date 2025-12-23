// game_data.h

#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "types.h"
#include <stdint.h>

// === GAME STATE SUB SYSTEMS ===

typedef struct {

    uint8_t current_scene;
    uint8_t keys;
    uint8_t previous_keys;
    uint8_t paused;

} System;

typedef struct {
    
    // === GRAPHICS DATA ===
    unsigned char *active_background_tilemap;
    unsigned char *active_background_tileset;
    unsigned char *active_sprite_sheet;
    unsigned char *active_font;

    // === MEMORY TRACKING ===
    // fonts
    uint16_t basic_font_vram_start_location;
    // Sprites
    uint16_t next_sprite_slot;
    uint8_t sprite_count;
    // Background
    uint16_t next_background_tile_slot;

} Graphics;

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;             // Total active objects
    
    // Registries (indices into pool for fast iteration by type)
    GameObject* ball_pointers[MAX_BALLS];
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
void clear_game_state(GameState* game);


#endif // GAME_STATE_H_