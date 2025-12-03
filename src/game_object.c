// game_object.c

#include "game_object.h"

// Pseudocode for spawning
GameObject* spawn_ball(void) {
    // 1. Find the next available FREE index (or use a free list)
    uint8_t pool_index = find_free_slot(game_object_pool); 
    
    // 2. Initialize the GameObject in the pool
    GameObject* new_obj = &game_object_pool[pool_index];
    new_obj->object_type = OBJ_BALL;
    // ... initialize other generic and ball-specific data

    // 3. Register the index in the type-specific array
    ball_indices[num_balls] = pool_index;
    num_balls++; // Increment the count of active balls
    
    return new_obj;
}
