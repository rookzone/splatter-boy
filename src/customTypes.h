// customTypes.h

#include <gb/gb.h>

#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

typedef struct GameSprite GameSprite;

// ***** FIXED NUMBERS *****

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
#define FIXED_MUL(a,b) (((a) * (b)) >> FIXED_SHIFT)
#define FIXED_DIV(a,b) (((a) << FIXED_SHIFT) / (b))


// === STATES ===

#define STATE_GAME_SCREEN 1
#define STATE_TITLE_SCREEN 2
#define STATE_SCORE_SCREEN 3
#define STATE_DEMO_SCREEN 4


// ##### GAME OBJECTS #####

// === Interactable objects ===

typedef struct {
    uint8_t x, y;
    fixed_n sub_x, sub_y;
    fixed_n  vx, vy;
    GameSprite *game_sprite;
} Ball;

typedef struct {
    uint8_t x, y;
    GameSprite *game_sprite;
} Wall;

typedef struct {
    uint8_t x, y;
    GameSprite* game_sprite;
} Pin;

// === Graphics ===

// General gameboy graphics sizes
#define TILE_WIDTH 8
#define TILE_LENGTH 8
#define TILE_HALF_WIDTH 4
#define TILE_HALF_LENGTH 4
#define SPRITE_SIZE 8
#define MAP_WIDTH_TILES 20
#define MAP_HEIGHT_TILES 18
#define MAP_WIDTH_PIXELS 160
#define MAP_HEIGHT_PIXELS 144


// This stores the tile the sprite uses and crucially holds the sprite ID for where it is loaded into VRAM
typedef struct GameSprite {
    uint8_t sprite_index;
    uint8_t tile_index;
};

// === LOOKUP TABLES ===

static const fixed_n RANDOM_HORIZONTAL_VX[10] = {
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50    // -0.5
};

#endif

