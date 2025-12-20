// game_object.h

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "types.h"

GameObject* go_spawn_object(ObjectType type);

void go_update_all_balls(void);

void go_draw_all_balls(void);

// Return ball GameObject defined at position index using ball registry
GameObject* go_return_ball(uint8_t index);

#endif // GAME_OBJECT_H_