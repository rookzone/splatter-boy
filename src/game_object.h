// game_object.h

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <gb/gb.h>
#include "custom_types.h"

#define MAX_GAME_OBJECTS 32 
#define MAX_BALLS 16
#define MAX_PINS 16

// Forward declaration for the function pointer
typedef struct GameObject GameObject; 

struct GameObject {

    uint8_t id;
    uint8_t flags;
    uint8_t active;
    ObjectType type;

    // Components

    TransformComponent transform;
    PhysicsComponent physics;
    RenderComponent renderer;

};

void go_init_manager(void);

GameObject* go_spawn_object(ObjectType type);

void go_update_all(void);

void go_draw_all(void);

// Return ball GameObject defined at position index using ball registry
GameObject* go_return_ball(uint8_t index);

#endif // GAME_OBJECT_H_