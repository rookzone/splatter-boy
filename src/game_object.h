// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "types.h"

GameObject* go_new_game_object(ObjectType type);

// Return ball GameObject defined at position index using ball registry
GameObject* go_get_ball(uint8_t index);

#endif // GAME_OBJECT_H
