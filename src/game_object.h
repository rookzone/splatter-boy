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

// Define function pointer type
typedef void (*UpdateFunc)(GameObject *obj); 

// Renamed Registry struct (Internal to the module)
typedef struct {
    uint8_t ball_indices[MAX_BALLS];
    uint8_t ball_count;
} ObjectIndices;

struct GameObject {
    // Position (Generic)
    uint8_t x, y;

    // Type
    ObjectType object_type;
    
    // Active Flag
    uint8_t active; 

    // Function pointer to update function (different depending on object type)
    UpdateFunc update;

    // Sprite Data
    GameSprite sprite;

    // Specific Data Union
    union {
        Ball ball;
        Pin pin;
    } data;

};


void go_init_manager(void);

GameObject* go_spawn_object(ObjectType type);

void go_update_all(void);

void go_draw_all(void);

#endif // GAME_OBJECT_H_