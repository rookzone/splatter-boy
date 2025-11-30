// gamestate.h

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <gb/gb.h>
#include "customTypes.h"


// Ball values
#define NUM_BALLS 16
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


#endif //GAMESTATE_H