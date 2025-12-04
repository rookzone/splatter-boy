/**
 * @file game_object.h
 * @brief Manage game object functions and lists
 * * @details This module manages game objects (GameObject).
 * A game object is a structure that is used to represent any game piece. It has some generic attributes shared across objects.
 * It also contains a union for a more specific object struct such as Ball or Pin.
 * The GameObject also stores function pointers that can be assigned to more specific functions.
 * This enables the handling of objects and loop updates in game states to be seperate from implimentation of specific objects.
 * * @defgroup Game Object Data
 */

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <gb/gb.h>
#include "custom_types.h"

// Max number of objects supported (adjust based on game/memory)
#define MAX_GAME_OBJECTS 32 
#define MAX_BALLS 16
#define MAX_PINS 16

// Define function pointer type so object can point to it's specific update function
typedef void (*UpdateFunc)(GameObject *obj); 

typedef enum {
    OBJ_BALL,
    OBJ_ENEMY,
    OBJ_COIN
} ObjectType;

typedef struct{

    // Position
    uint8_t x, y;

    // For reference
    ObjectType object_type;

    // Points to a function defined in the object types specific code.
    UpdateFunc update;

    // gfx data
    GameSprite* sprite;

    // A list of more specific object structures.
    // Can be accessed like this `GameObject.data.specific_object.specific_value`
    union{
        Ball ball;
    } data;


} GameObject;


#endif