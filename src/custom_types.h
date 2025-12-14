// custom_types.h

#ifndef CUSTOM_TYPES_H_
#define CUSTOM_TYPES_H_

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

// === GAME OBJECT ===

// === COMPONENT FLAGS ===
#define OBJECT_ACTIVE    0x01  // Object is alive
#define TRANSFORM_ACTIVE 0x02  // Has position
#define PHYSICS_ACTIVE   0x04  // Has velocity/physics
#define RENDERER_ACTIVE    0x08  // Has sprite/should render

// ** Components **
typedef struct{

    fixed_n vx, vy; // Velocity
    fixed_n fractional_vx, fractional_vy; // Velocity fractional accumulater (sub-pixel)
    uint8_t collision_enabled; // 1 = enable collision, 0 = ignore collision

} PhysicsComponent;

typedef struct{

    uint8_t sprite_index; // the index this sprite was initialised at
    uint8_t tile_index;
    uint8_t visible;

} RenderComponent;

typedef struct {

    uint8_t x, y; // Position in pixels
    
} TransformComponent;

// ** Flags and Settings **

// Create definitions for the different object types
typedef enum {
    OBJ_BALL,
    OBJ_PIN,
    OBJ_SLANTED_WALL,
    OBJ_GENERAL,
    OBJ_SPINNER
} ObjectType;

// === Graphics ===

struct GameSprite {
    uint8_t sprite_index;
    uint8_t tile_index;
};

#endif // CUSTOM_TYPES_H_