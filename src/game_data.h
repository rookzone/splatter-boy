// game_data.h

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <gb/gb.h>
#include "custom_types.h"
#include "game_object.h"

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
    uint8_t active_balls_indices[MAX_BALLS];
    uint8_t ball_count; // Current next available index


} ObjectIndices;

typedef struct {

    ObjectIndices indices; // WHAT NAME???
    GameObject game_object_pool[MAX_GAME_OBJECTS];
    uint8_t total_object_count;

} ObjectManager;

typedef struct {

    System system;
    Graphics graphics;
    ObjectManager objects;

} GameState;


extern GameState game;


#endif // GAME_DATA_H_
