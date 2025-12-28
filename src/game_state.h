// game_state.h

#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "types.h"

// === GAME STATE SUB SYSTEMS ===

typedef struct {

    uint8_t current_scene;
    uint8_t keys;
    uint8_t previous_keys;
    uint8_t paused;
    uint16_t system_time;
    uint16_t sys_time_since_state_load;


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
    uint16_t active_background_tileset_size;
    uint16_t background_vram_start_location;

} Graphics;

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;
    
    // Game object pointer registries
    GameObject* ball_pointers[MAX_GAME_OBJECTS];
    uint8_t ball_count;

    GameObject* generic_pointers[MAX_GAME_OBJECTS];
    uint8_t generic_count;

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

void update_game_state(void);


#endif // GAME_STATE_H_