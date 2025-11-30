// customTypes.h

#include <gb/gb.h>

#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

// Forward declaration of GameSprite for object definitions
typedef struct GameSprite GameSprite;

// ***** FIXED NUMBERS *****

// 16-bit signed integer for 8.8 fixed-point numbers
typedef int16_t fixed_n;   

#define FIXED_SHIFT   8           // number of fractional bits
#define FIXED_ONE     (1 << FIXED_SHIFT)   // 1.0 in fixed (256)
#define FIXED_HALF    (1 << (FIXED_SHIFT - 1)) // 0.5 (128)
#define FIXED_QUARTER (1 << (FIXED_SHIFT - 2)) // 0.25 (64)
#define FIXED_EIGHTH  (1 << (FIXED_SHIFT - 3)) // 0.125 (32)
#define FIXED_TEENTH  (1 << (FIXED_SHIFT - 4)) // (16)
#define FIXED_TENBAG  (1 << (FIXED_SHIFT - 5)) // (8)


// === CONVERSION HELPERS ===
#define TO_FIXED(x)   ((fixed_n)((x) << FIXED_SHIFT))       // int -> fixed
#define FROM_FIXED(x) ((x) >> FIXED_SHIFT)                // fixed -> int
#define FIXED_ADD(a,b) ((a) + (b))
#define FIXED_SUB(a,b) ((a) - (b))

// === SLOW MATHS ===
#define FIXED_MUL(a,b) (((a) * (b)) >> FIXED_SHIFT) // fixed * fixed -> fixed
#define FIXED_DIV(a,b) (((a) << FIXED_SHIFT) / (b)) // fixed / fixed -> fixed

// === SPRITE/TILE CONSTANTS ===
#define SPRITE_SIZE 8 // 8x8 pixels
#define TILE_HALF_WIDTH 4 // 8/2 = 4

// ##### GAME OBJECTS #####

// === Interactable objects ===

// Ball object
typedef struct {
    uint8_t x, y;
    fixed_n sub_x, sub_y; // Fractional part of position (sub-pixel)
    fixed_n  vx, vy;      // Velocity
    GameSprite *game_sprite;
} Ball;

// Wall object
typedef struct {
    uint8_t x, y; // Top-left corner of the wall section
    GameSprite *game_sprite;
} Wall;

// Pin object
typedef struct {
    uint8_t x, y;
    GameSprite* game_sprite;
} Pin;


// === Graphics ===

// This stores the tile the sprite uses and crucially holds the sprite index for where it is loaded into VRAM
typedef struct GameSprite {
    uint8_t sprite_index;
    uint8_t tile_index;
};

// Byte that represents the PIN. Used for collision checks
#define PIN_TILE_ID 0x02

// Convert Pixel coordinate to Grid coordinate (Divide by 8)
#define PIXEL_TO_GRID(x) ((x) >> 3)

// Convert Grid coordinate to Pixel coordinate (Multiply by 8)
#define GRID_TO_PIXEL(x) ((x) << 3)

// Get tilemap Array Index (assuming 20 tiles wide)
#define GET_TILE_INDEX(col, row) ((row) * 20 + (col))

#endif // CUSTOMTYPES_H