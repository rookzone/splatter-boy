// game_object.c

#include "game_object.h"
#include "game_state.h"
#include "graphics.h"
#include "ball.h"
#include <string.h>

// === GAME OBJECT HANDLING ===

// Spawn an object of ObjectType.
// Creates GameObject and assigns it to the pool
// Registers the object with it's specific registry function to keep track of index values
// Incriment index values ready for next object
GameObject* go_new_game_object(ObjectType type) {

    /**
     * @todo handle max objects
     */
    if (game.objects.total_count >= MAX_GAME_OBJECTS) {
        return NULL; 
    }

    uint8_t pool_index = game.objects.total_count;
    GameObject* obj = &game.objects.pool[pool_index];

    // Zero out object values in case any junk values exist.
    memset(obj, 0, sizeof(GameObject));
    
    // Set up object values
    obj->id = pool_index;
    obj->flags = OBJECT_ACTIVE;
    obj->type = type;
    
    // Register objects in their type specific registries
    if (type == OBJ_BALL && game.objects.ball_count < MAX_GAME_OBJECTS) {
        game.objects.ball_pointers[game.objects.ball_count] =  &game.objects.pool[pool_index];
        game.objects.ball_count++;
    }

    // Register in type-specific registry
    if (type == OBJ_GENERIC && game.objects.generic_count < MAX_GAME_OBJECTS) {
        game.objects.generic_pointers[game.objects.generic_count] =  &game.objects.pool[pool_index];
        game.objects.generic_count++;
    }

    // Change to switch case when more objects are built
    
    game.objects.total_count++;
    return obj;
}

void go_update_all_balls(void) {

    // Iterate through ball registry
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
           update_ball(game.objects.ball_pointers[i]);
    }
}

// Loop through all the registered game objects and draw them

void go_draw_all_balls(void) {

    for (uint8_t i = 0; i < game.objects.ball_count; i++) {

        GameObject* obj = game.objects.ball_pointers[i];
        
        if ((obj->flags & RENDERER_ACTIVE) == RENDERER_ACTIVE) {
            DRAW_SPRITE(&obj->renderer, obj->transform.x, obj->transform.y);
        }
    }
}

GameObject* go_return_ball(uint8_t index) {

    if (index < game.objects.ball_count) {
        return game.objects.ball_pointers[index];
    }
    return NULL;
}

/* End of game_object.c */