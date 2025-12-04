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

// Max number of objects
#define MAX_GAME_OBJECTS 64
#define MAX_BALLS 32

// The main pool where all object data lives
GameObject game_object_pool[MAX_GAME_OBJECTS];

// Separate arrays to store the indices (in the pool) of specific object types
uint8_t ball_indices[MAX_BALLS];
uint8_t num_balls = 0; // Tracks the current count

uint8_t find_free_slot(void);

// This function will check the game_object array and update the list registries with the correct indexing data.
void update_object_list_registry(void);

// Spawning
GameObject* spawn_ball(void);


#endif