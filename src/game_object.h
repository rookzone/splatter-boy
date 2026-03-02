// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "types.h"
#include "game_state.h"
#include <stddef.h>

GameObject* go_new_game_object(ObjectType type);

// Generic registry access (type-based)
static inline ObjectRegistry* go_get_registry(ObjectType type)
{
    static ObjectRegistry empty = { {0}, 0 };
    if (type >= OBJ_TYPE_COUNT)
        return &empty;
    return &game.objects.registries[type];
}

static inline uint8_t go_get_count(ObjectType type)
{
    ObjectRegistry* reg = go_get_registry(type);
    return reg->count;
}

static inline GameObject* go_get_by_type(ObjectType type, uint8_t index)
{
    ObjectRegistry* reg = go_get_registry(type);
    if (index >= reg->count)
        return NULL;
    return reg->pointers[index];
}

#endif // GAME_OBJECT_H
