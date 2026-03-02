// game_object.c

#include "game_object.h"
#include "game_state.h"
#include <stddef.h>
#include <string.h>
#include <gbdk/emu_debug.h>

// === GAME OBJECT HANDLING ===

// Spawn an object of ObjectType.
// Creates GameObject and assigns it to the pool
// Registers the object with it's specific registry function to keep track of their pointers
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
    
    // Register object in type-specific registry (generic)
    if (type < OBJ_TYPE_COUNT) {
        ObjectRegistry* reg = &game.objects.registries[type];
        if (reg->count < MAX_GAME_OBJECTS) {
            reg->pointers[reg->count] = &game.objects.pool[pool_index];
            reg->count++;
        }
    }

    // Change to switch case when more objects are built
    
    game.objects.total_count++;
    return obj;
}

ObjectRegistry* go_get_registry(ObjectType type)
{
    if (type >= OBJ_TYPE_COUNT)
        return NULL;
    return &game.objects.registries[type];
}

uint8_t go_get_count(ObjectType type)
{
    ObjectRegistry* reg = go_get_registry(type);
    return reg ? reg->count : 0;
}

GameObject* go_get_by_type(ObjectType type, uint8_t index)
{
    ObjectRegistry* reg = go_get_registry(type);
    if (!reg || index >= reg->count)
        return NULL;
    return reg->pointers[index];
}

/* End of game_object.c */
