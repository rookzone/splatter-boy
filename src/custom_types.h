// custom_types.h

#ifndef CUSTOM_TYPES_H_
#define CUSTOM_TYPES_H_

#include <gb/gb.h>

typedef struct GameSprite GameSprite;

// ***** FIXED NUMBERS *****
// These are a way to represent decimal numbers
// We use a 16bit integer and shift the 8 bit integer to left byte
// right byte represents a decimal in 1/256ths

typedef int16_t fixed_n;   

#define FIXED_SHIFT   8           // number of fractional bits
#define FIXED_ONE     (1 << FIXED_SHIFT)   // 1.0 in fixed (256)
#define FIXED_HALF    (1 << (FIXED_SHIFT - 1)) // 0.5 (128)     << 7
#define FIXED_QUARTER (1 << (FIXED_SHIFT - 2)) // 0.25 (64)     << 6
#define FIXED_EIGHTH  (1 << (FIXED_SHIFT - 3)) // 0.125 (32)    << 5
#define FIXED_TEENTH  (1 << (FIXED_SHIFT - 4)) // (16)          << 4
#define FIXED_TENBAG  (1 << (FIXED_SHIFT - 5)) // (8)           << 3


// === CONVERSION HELPERS ===
#define TO_FIXED(x)   ((fixed_n)((x) << FIXED_SHIFT))       // int -> fixed
#define FROM_FIXED(x) ((x) >> FIXED_SHIFT)                // fixed -> int
#define FIXED_ADD(a,b) ((a) + (b))
#define FIXED_SUB(a,b) ((a) - (b))

// === SLOW MATHS ===
#define FIXED_MUL(a,b) (((a) * (b)) >> FIXED_SHIFT)
#define FIXED_DIV(a,b) (((a) << FIXED_SHIFT) / (b))


// ##### GAME OBJECTS #####

/** === Interactable objects ===
* These are specific game object types for specific attributes
*/

// Ball object
typedef struct {
    uint8_t x, y;
    fixed_n sub_x, sub_y;
    fixed_n  vx, vy;
    GameSprite *game_sprite;
} Ball;

// Ball object
typedef struct {
    fixed_n sub_x, sub_y;
    fixed_n  vx, vy;
} Ball_new;

// Wall object
typedef struct {
    uint8_t x, y;
    GameSprite *game_sprite;
} Wall;

// Pin object
typedef struct {
    uint8_t x, y;
    GameSprite* game_sprite;
} Pin;


// === Graphics ===

struct GameSprite {
    uint8_t sprite_index;
    uint8_t tile_index;
};

/** === Game Object definitons ===
 *  Generic GameObject.
 */

typedef struct GameObject GameObject;

typedef void (*UpdateFunc)(GameObject *obj);

typedef enum {
    OBJ_BALL,
    OBJ_ENEMY,
    OBJ_COIN
} ObjectType;

struct GameObject{ 
    
    // Position
    uint8_t x, y;

    // For reference
    ObjectType object_type;

    // Points to a function defined in the object types specific code.
    UpdateFunc update;

    GameSprite* sprite;

    // A list of more specific object structures.
    union{
        Ball_new ball;
    } data;
};



#endif // CUSTOM_TYPES_H_

