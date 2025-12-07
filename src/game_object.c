// game_object.c
#include "game_object.h"
#include "graphics.h"
#include "ball.h"
#include <string.h>


static GameObject game_object_pool[MAX_GAME_OBJECTS];
static uint8_t total_object_count = 0;
static ObjectIndices indices;

// === GAME OBJECT HANDLING ===

// Initialise the manager, reset the index values to 0 for re-writing
void go_init_manager(void) {
    total_object_count = 0;
    indices.ball_count = 0;
    // @todo set active to 0 (for when active is used)
}

// Spawn an object of ObjectType.
// Creates GameObject and assigns it to the pool
// Registers the object with it's specific registry function to keep track of index values
// Incriment index values ready for next object
GameObject* go_spawn_object(ObjectType type) {
    
    if (total_object_count >= MAX_GAME_OBJECTS) {
        return NULL; // Pool full
    }

    uint8_t pool_index = total_object_count; 
    GameObject* new_object = &game_object_pool[pool_index];
    
    new_object->object_type = type;
    new_object->active = 1;

    if (type == OBJ_BALL && indices.ball_count < MAX_BALLS) {
        indices.ball_indices[indices.ball_count] = pool_index; 
        indices.ball_count++;
    }
    
    total_object_count++;
    
    return new_object;
}

// Loops through the objects running their associated update functions
void go_update_all(void) {

    // Iterate through all ball objects and run each update function
    for (uint8_t i = 0; i < indices.ball_count; i++) {
        uint8_t pool_index = indices.ball_indices[i];
        GameObject* obj = &game_object_pool[pool_index];
        
        if (obj->active && obj->update != NULL) {
            obj->update(obj);
        }
    }
}

// Loop through all the registered game objects and draw them
void go_draw_all(void) {
     for (uint8_t i = 0; i < indices.ball_count; i++) {
        uint8_t pool_index = indices.ball_indices[i];
        GameObject* obj = &game_object_pool[pool_index];
        
        
        if (obj->active) {
            // Access the generic sprite struct
           DRAW_SPRITE(&obj->sprite, obj->x, obj->y);
        }
    }
}

GameObject* go_return_ball(uint8_t index)
{
    if (index < MAX_BALLS)
        return &game_object_pool[indices.ball_indices[index]];
    
    return NULL;
    
}