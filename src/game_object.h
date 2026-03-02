// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "types.h"
#include "game_state.h"
#include <stddef.h>

GameObject* go_new_game_object(ObjectType type);

// Generic registry access (type-based)
ObjectRegistry* go_get_registry(ObjectType type);
uint8_t go_get_count(ObjectType type);
GameObject* go_get_by_type(ObjectType type, uint8_t index);

#endif // GAME_OBJECT_H
