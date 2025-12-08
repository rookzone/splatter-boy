// game_data.h

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <gb/gb.h>
#include "custom_types.h"
#include "game_object.h"

// Game Object list sizes
#define NUM_BALLS 18 // HIGHER THAN 18 LAGS - DO NOT SET TO ODD NUMBER
#define NUM_PINS 36

typedef struct {

    uint8_t current_state;
    uint8_t keys;
    uint8_t previous_keys;

} System;

typedef struct {
    
    unsigned char *active_background_tilemap;
    unsigned char *active_background_tiledata;
    unsigned char *active_sprite_sheet;
    uint16_t next_sprite_id;

} Graphics;

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;             // Total active objects
    
    /** === REGISTRIES ===
     * These registries are used to hold the index (location) of a certain type of GameObject
     * On creation an object should register it's index in the main object pool into this array.
     * This allows for parts of the game to iterate through only this object type without expensive object type checks
     */
    uint8_t ball_indices[MAX_BALLS];
    uint8_t ball_count;

} ObjectManager;

typedef struct {

    System system;
    Graphics graphics;

} GameState;


extern GameState game;


#endif // GAME_DATA_H_