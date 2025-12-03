// game_data.h

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <gb/gb.h>
#include "custom_types.h"

// Game Object list sizes
#define NUM_BALLS 18 // HIGHER THAN 18 LAGS - DO NOT SET TO ODD NUMBER
#define NUM_PINS 36

#define MAX_OBJECTS 64 // Max number of GameObject


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
    Ball list[NUM_BALLS];
    GameSprite graphics[NUM_BALLS];
} BallObjects;

typedef struct {
    Pin list[NUM_PINS];
    GameSprite graphics[NUM_PINS];
} PinObjects;

typedef struct {

    BallObjects balls;
    PinObjects pins;

} GameObjects;

typedef struct {

    System system;
    Graphics graphics;
    GameObjects objects;

} GameState;


extern GameState game;


#endif //GAME_DATA_H_

