// types.h

#ifndef types_H_
#define types_H_

#include <stdint.h>

typedef struct GameSprite GameSprite;

// ***** FIXED NUMBERS *****
// These are a way to represent decimal numbers
// We use a 16bit integer and shift the 8 bit integer to left byte
// right byte represents a decimal in 1/256ths

typedef int16_t fixed_t;   

#define FIXED_SHIFT   8           // number of fractional bits
#define FIXED_ONE     (1 << FIXED_SHIFT)   // 1.0 in fixed (256)
#define FIXED_HALF    (1 << (FIXED_SHIFT - 1)) // 0.5 (128)     << 7
#define FIXED_QUARTER (1 << (FIXED_SHIFT - 2)) // 0.25 (64)     << 6
#define FIXED_EIGHTH  (1 << (FIXED_SHIFT - 3)) // 0.125 (32)    << 5
#define FIXED_TEENTH  (1 << (FIXED_SHIFT - 4)) // (16)          << 4
#define FIXED_TENBAG  (1 << (FIXED_SHIFT - 5)) // (8)           << 3


// === CONVERSION HELPERS ===
#define TO_FIXED(x)   ((fixed_t)((x) << FIXED_SHIFT))       // int -> fixed
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

    fixed_t vx, vy; // Velocity
    fixed_t fractional_vx, fractional_vy; // Velocity fractional accumulater (sub-pixel)
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

typedef struct {

    uint8_t id;
    uint8_t flags;
    ObjectType type;

    // Components

    TransformComponent transform;
    PhysicsComponent physics;
    RenderComponent renderer;

} GameObject;


// Constants THIS IS REALLY BAD LOL

#define NUM_BALLS 18

#define LAUNCH_FORCE_X TO_FIXED(2)
#define LAUNCH_FORCE_Y TO_FIXED(2)

#define MAX_GAME_OBJECTS 40
#define MAX_BALLS 20

// Tile and sprite sizes
#define TILE_WIDTH          8
#define TILE_LENGTH         8
#define TILE_HALF_WIDTH     4
#define TILE_HALF_HEIGHT    4
#define TILE_HALF_LENGTH    4

#define PIN_HALF_WIDTH  3
#define SPRITE_SIZE     8
#define NUM_HW_sPRITES  40

#define BACKGROUND_WIDTH_TILES      20
#define BACKGROUND_HEIGHT_TILES     18
#define BACKGROUND_WIDTH_PIXELS     160
#define BACKGROUND_HEIGHT_PIXELS    144

// Tileset sizes
#define BACKGROUND_TILESET_SIZE     16
#define MENU_FONT_TILESET_SIZE      36

// Offsets for font data
#define LOWER_CASE_ASCII_OFFSET 'a'
#define UPPER_CASE_ASCII_OFFSET 'A'
#define NUMBER_ASCII_OFFSET '0'

// Friendly name that corresponds with tile position in sprite sheet(s). 
enum { TILE_BALL = 0, TILE_WALL = 1, TILE_PIN = 2 };

// === STATES ===

#define STATE_TITLE_SCREEN 0
#define STATE_GAME_SCREEN 1
#define STATE_GAME2_SCREEN 2
#define STATE_SCORE_SCREEN 3
#define STATE_DEMO_SCREEN 4

// Physics constants (8.8 fixed-point)
#define GRAVITY     FIXED_TEENTH  // 256/16
#define MAX_SPEED   TO_FIXED(2) // Max speed of balls in fixed-point number space
#define ROLL_FORCE FIXED_QUARTER // Amount of force applied to ball to continue roll
#define MAX_ROLL_SPEED TO_FIXED(2) // Max speed ball can go while rolling
#define HORIZONTAL_PIN_FORCE FIXED_QUARTER // For bounce force on x-axis


#endif // types_H_