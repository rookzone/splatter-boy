// constants.h

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// Game object limits (defines fixed array size)
#define MAX_GAME_OBJECTS    40
#define MAX_BALLS           30

// How many to spawn in
#define NUM_BALLS 10

// Hardware values for GB
#define NUM_HW_sPRITES  40

// Tile measurements in px
#define TILE_WIDTH          8
#define TILE_LENGTH         8
#define TILE_HALF_WIDTH     4
#define TILE_HALF_HEIGHT    4
#define TILE_HALF_LENGTH    4

// Sprite values
#define PIN_HALF_WIDTH  3
#define SPRITE_SIZE     8

// Background px and tiles for GB
#define BACKGROUND_WIDTH_TILES      20
#define BACKGROUND_HEIGHT_TILES     18
#define BACKGROUND_WIDTH_PIXELS     160
#define BACKGROUND_HEIGHT_PIXELS    144

// Tileset sizes
#define BACKGROUND_TILESET_SIZE     16
#define MENU_FONT_TILESET_SIZE      42
#define BASIC_FONT_TILESET_SIZE     42

// Offsets for font data
#define LOWER_CASE_ASCII_OFFSET     'a'
#define UPPER_CASE_ASCII_OFFSET     'A'
#define NUMBER_ASCII_OFFSET         '0'

// Friendly name that corresponds with tile position in sprite sheet(s). 
enum { TILE_BALL = 0, TILE_WALL = 1, TILE_PIN = 2 };

// Physics constants (8.8 fixed-point)

// Ball launcher
#define LAUNCH_FORCE_X  TO_FIXED(2)
#define LAUNCH_FORCE_Y  TO_FIXED(2)

#define GRAVITY     FIXED_TEENTH  // 256/16
#define MAX_SPEED   TO_FIXED(2) // Max speed of balls in fixed-point number space

// Ball physics
#define ROLL_FORCE              FIXED_QUARTER // Amount of force applied to ball to continue roll
#define MAX_ROLL_SPEED          TO_FIXED(2) // Max speed ball can go while rolling
#define HORIZONTAL_PIN_FORCE    FIXED_QUARTER // For bounce force on x-axis

#endif // CONSTANTS_H_