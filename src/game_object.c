// game_object.c

#include "game_object.h"
#include "graphics.h"
#include "ball.h"
#include <string.h>

// === GAME OBJECT HANDLING ===

// Initialise the manager, reset the index values to 0 for re-writing
void go_init_manager(void) {
    game.objects.total_count = 0;
    game.objects.ball_count = 0;
}

// Spawn an object of ObjectType.
// Creates GameObject and assigns it to the pool
// Registers the object with it's specific registry function to keep track of index values
// Incriment index values ready for next object
GameObject* go_spawn_object(ObjectType type) {
    if (game.objects.total_count >= MAX_GAME_OBJECTS) {
        return NULL; // Pool full
    }

    uint8_t pool_index = game.objects.total_count;
    GameObject* obj = &game.objects.pool[pool_index];
    
    // Clear the object
    obj->active = 1;
    obj->id = pool_index;
    obj->flags = COMP_ACTIVE;
    obj->type = type;
    
    // Register in type-specific registry
    if (type == OBJ_BALL && game.objects.ball_count < MAX_BALLS) {
        game.objects.ball_indices[game.objects.ball_count] = pool_index;
        game.objects.ball_count++;
    }
    
    game.objects.total_count++;
    return obj;
}

void go_update_all(void) {

    // Iterate through ball registry
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
        uint8_t pool_index = game.objects.ball_indices[i];
        GameObject* obj = &game.objects.pool[pool_index];
        
        if (obj->active == 1) { // TODO - NEED TO RETHINK UPDATE, FUNC POINTER IS TOO SLOW
           // obj->update(obj); UPDATE
        }
    }
}

// Loop through all the registered game objects and draw them


void go_draw_all(void) {

    for (uint8_t i = 0; i < game.objects.ball_count; i++) {

        uint8_t pool_index = game.objects.ball_indices[i]; // Grab object pool index from ball register
        GameObject* obj = &game.objects.pool[pool_index];
        
        if ((obj->flags & (COMP_ACTIVE | COMP_RENDER)) == (COMP_ACTIVE | COMP_RENDER)) {
            DRAW_SPRITE(&obj->renderer, obj->transform.x, obj->transform.y);
        }
    }
}

GameObject* go_return_ball(uint8_t index) {
    if (index < game.objects.ball_count) {
        uint8_t pool_index = game.objects.ball_indices[index];
        return &game.objects.pool[pool_index];
    }
    return NULL;
}

/* End of game_object.c */