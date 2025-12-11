// game_object.h

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <gb/gb.h>
#include "custom_types.h"

#define MAX_GAME_OBJECTS 40
#define MAX_BALLS 20

// Forward declaration for the function pointer
typedef struct GameObject GameObject; 

struct GameObject {

    uint8_t id;
    uint8_t flags;
    ObjectType type;

    // Components

    TransformComponent transform;
    PhysicsComponent physics;
    RenderComponent renderer;

};

GameObject* go_spawn_object(ObjectType type);

void go_update_all_balls(void);

void go_draw_all_balls(void);

// Return ball GameObject defined at position index using ball registry
GameObject* go_return_ball(uint8_t index);

#endif // GAME_OBJECT_H_