// ball.c

#include "ball.h"
#include "graphics.h"
#include "physics.h"
#include "game_object.h"


GameObject* spawn_ball(uint8_t x, uint8_t y) {

    GameObject* obj = go_spawn_object(OBJ_BALL);
    if (obj == NULL) return NULL;
    
    // Configure components
    obj->flags |= (TRANSFORM_ACTIVE | PHYSICS_ACTIVE | RENDERER_ACTIVE);
    
    // Transform
    obj->transform.x = x;
    obj->transform.y = y;
    
    // Physics
    obj->physics.vx = 0;
    obj->physics.vy = 0;
    obj->physics.fractional_vx = 0;
    obj->physics.fractional_vy = 0;
    obj->physics.collision_enabled = 1;
    
    // Render
    GameSprite sprite = create_sprite(TILE_BALL);
    obj->renderer.sprite_index = sprite.sprite_index;
    obj->renderer.tile_index = sprite.tile_index;
    obj->renderer.visible = 1;
    
    return obj;
}


void update_ball(GameObject* obj) {

    // Quick validation
    if (!(obj->flags & PHYSICS_ACTIVE)) return;
    
    // Handle collision (pass GameObject directly)
    handle_ball_pin_collision(obj);
    
    // Update position (pass GameObject directly)
    update_ball_position(obj);
    
}

void reset_all_balls(void) {
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
        GameObject* obj = go_return_ball(i);
        if (obj == NULL) continue;
        
        // Reset position
        if (i < NUM_BALLS >> 1) {
            obj->transform.x = 10 + i * 8;
            obj->transform.y = 20;
        } else {
            obj->transform.x = 10 + (i - 8) * 8;
            obj->transform.y = 30;
        }
        
        // Reset physics
        obj->physics.vx = RANDOM_HORIZONTAL_VX[i];
        obj->physics.vy = 0;
        obj->physics.fractional_vx = 0;
        obj->physics.fractional_vy = 0;
    }
}


/* NOT YET REFACTORED TO USE GENERIC GAME OBJECT SYSTEM

void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y)
{
    ball->vx = 0;
    ball->vy = 0;
    ball->sub_x = 0;
    ball->sub_y = 0;
    ball->x = from_x;
    ball->y = from_y;
    apply_impulse(ball, launch_power_x, launch_power_y);
}
*/

/*

Ball* find_lowest_ball(Ball* balls, uint8_t count)
{
    if (count == 0) {
        return NULL; // Return NULL if the array is empty
    }

    Ball* lowest_ball = &balls[0];
    uint8_t highest_y = balls[0].y;

    for (uint8_t i = 1; i < count; i++) {
        // Compare the current ball's y-coordinate with the maximum found so far
        if (balls[i].y > highest_y) {
            highest_y = balls[i].y;
            lowest_ball = &balls[i];
        }
    }

    return lowest_ball;
}
    */

/* End of ball.c */
// ball.h


#ifndef BALL_H_
#define BALL_H_

#include "custom_types.h"
#include "game_object.h"
#include "game_data.h"

#define LAUNCH_FORCE_X TO_FIXED(2)
#define LAUNCH_FORCE_Y TO_FIXED(2)

GameObject* spawn_ball(uint8_t x, uint8_t y);

void update_ball(GameObject* obj);

void reset_all_balls(void);

#endif // BALL_H_
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

// === GAME OBJECT ===

// === COMPONENT FLAGS ===
#define COMP_ACTIVE    0x01  // Object is alive
#define COMP_TRANSFORM 0x02  // Has position
#define COMP_PHYSICS   0x04  // Has velocity/physics
#define COMP_RENDER    0x08  // Has sprite/should render

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
// debug.c

#include "debug.h"
#include <gbdk/emu_debug.h>

void debug_print_data(void)
{
    // code
    
    
}

/* End of debug.c */
// debug.h

#ifndef DEBUG_H_
#define DEBUG_H_

#include "custom_types.h"


// Pauses game - shows screen with values in GameState game (game_data.c/.h)
void debug_print_data(void);

//
uint16_t get_execution_time(void);

#endif // DEBUG_H_
// game_data.c

#include "game_data.h"

GameState game;

/* End of game_data.c */
// game_data.h

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <gb/gb.h>
#include "custom_types.h"
#include "game_object.h"

#define NUM_BALLS 18
#define NUM_PINS 36

// === GAME STATE SUB SYSTEMS ===

typedef struct {

    uint8_t current_state;
    uint8_t keys;
    uint8_t previous_keys;

} System;

typedef struct {
    
    unsigned char *active_background_tilemap;
    unsigned char *active_background_tiledata;
    unsigned char *active_sprite_sheet;
    uint16_t next_sprite_id;
    uint8_t sprite_count;

} Graphics;

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;             // Total active objects
    
    // Registries (indices into pool for fast iteration by type)
    uint8_t ball_indices[MAX_BALLS];
    uint8_t ball_count;

} ObjectManager;

// === GAME STATE ===

typedef struct {

    System system;
    Graphics graphics;
    ObjectManager objects;

} GameState;


extern GameState game;


#endif // GAME_DATA_H_
// game_object.c

#include "game_object.h"
#include "graphics.h"
#include "ball.h"
#include <string.h>

// === GAME OBJECT HANDLING ===

// Initialise the manager, reset the index values to 0 for re-writing
void go_init_manager(void) {
    game.objects.total_count = 0;
    game.objects.ball_count = 0;
}

// Spawn an object of ObjectType.
// Creates GameObject and assigns it to the pool
// Registers the object with it's specific registry function to keep track of index values
// Incriment index values ready for next object
GameObject* go_spawn_object(ObjectType type) {
    if (game.objects.total_count >= MAX_GAME_OBJECTS) {
        return NULL; // Pool full
    }

    uint8_t pool_index = game.objects.total_count;
    GameObject* obj = &game.objects.pool[pool_index];
    
    // Clear the object
    obj->active = 1;
    obj->id = pool_index;
    obj->flags = OBJECT_ACTIVE;
    obj->type = type;
    
    // Register in type-specific registry
    if (type == OBJ_BALL && game.objects.ball_count < MAX_BALLS) {
        game.objects.ball_indices[game.objects.ball_count] = pool_index;
        game.objects.ball_count++;
    }
    
    game.objects.total_count++;
    return obj;
}

void go_update_all_balls(void) {

    // Iterate through ball registry
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
        uint8_t pool_index = game.objects.ball_indices[i];
        GameObject* obj = &game.objects.pool[pool_index];
        
        if (obj->active == 1) { // TODO - NEED TO RETHINK UPDATE, FUNC POINTER IS TOO SLOW
           // obj->update(obj); UPDATE
        }
    }
}

// Loop through all the registered game objects and draw them


void go_draw_all_balls(void) {

    for (uint8_t i = 0; i < game.objects.ball_count; i++) {

        uint8_t pool_index = game.objects.ball_indices[i]; // Grab object pool index from ball register
        GameObject* obj = &game.objects.pool[pool_index];
        
        if ((obj->flags & (OBJECT_ACTIVE | RENDERER_ACTIVE)) == (OBJECT_ACTIVE | RENDERER_ACTIVE)) {
            DRAW_SPRITE(&obj->renderer, obj->transform.x, obj->transform.y);
        }
    }
}

GameObject* go_return_ball(uint8_t index) {
    if (index < game.objects.ball_count) {
        uint8_t pool_index = game.objects.ball_indices[index];
        return &game.objects.pool[pool_index];
    }
    return NULL;
}

/* End of game_object.c */
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
// graphics.c

#include "graphics.h"
#include "game_data.h"

// Create sprite with a tracked VRAM number, assign tile, create and return GameSprite object
GameSprite create_sprite(uint8_t tile_index)
{
    GameSprite new_sprite;
    new_sprite.sprite_index = game.graphics.next_sprite_id;
    new_sprite.tile_index = tile_index;
    set_sprite_tile(game.graphics.next_sprite_id, tile_index);
    game.graphics.next_sprite_id++;

    return new_sprite;
}

// Plot a point at position in fixed number space
void plot_point_fixed(fixed_n x, fixed_n y)
{
    plot_point(FROM_FIXED(x), FROM_FIXED(y));
}

void set_game_background(unsigned char *background, unsigned char *tiles)
{
    game.graphics.active_background_tilemap = background;
    game.graphics.active_background_tiledata = tiles;

    set_bkg_tiles(0, 0, 20, 18, game.graphics.active_background_tilemap);
    set_bkg_data(0, 255, game.graphics.active_background_tiledata);
}

unsigned char* get_game_background_tilemap(void)
{
    return game.graphics.active_background_tilemap;
}

void set_sprite_sheet(unsigned char *sprite_sheet)
{
    game.graphics.active_sprite_sheet = sprite_sheet;
    set_sprite_data(0, 16, game.graphics.active_sprite_sheet);
}

/* End of graphics.c */
/**
 * @file graphics.h
 * @brief Handles drawing of background, window or sprite tiles on screen.
 * * @details Contains useful macros for handling tile grids and pixels.
 * Functions to load tiles, backgrounds, and sprites. The GameSprite struct is used.
 * This contains the tile_index and sprite_index, these are indices that reference the ID in memory.
 * Memory is managed in here, so the logic can just ask for something to be drawn without worrying about IDs and memory.
 * The GameSprite object reference that is returned can be used to call graphics functions.
 * Passing around structs is slow, so try and keep that stuff out of any update loops.
 * * @defgroup Graphics
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <gb/drawing.h>
#include "custom_types.h"

// Byte that represents the PIN. Used for collision checks 
#define PIN_TILE_ID 0x02

// Convert Pixel coordinate to Grid coordinate (Divide by 8)
#define PIXEL_TO_GRID(x) ((x) >> 3)

// Convert Grid coordinate to Pixel coordinate (Multiply by 8)
#define GRID_TO_PIXEL(x) ((x) << 3)

// Get tilemap Array Index from Col/Row (Row * Width + Col)
#define GET_TILE_INDEX(col, row) (((uint16_t)(row) * BACKGROUND_WIDTH_TILES) + (col))

// Tile and sprite sizes
#define TILE_WIDTH 8
#define TILE_LENGTH 8
#define TILE_HALF_WIDTH 4
#define TILE_HALF_HEIGHT 4
#define TILE_HALF_LENGTH 4
#define PIN_HALF_WIDTH 3 // Half the width of the pin sprite.
#define SPRITE_SIZE 8
#define BACKGROUND_WIDTH_TILES 20
#define BACKGROUND_HEIGHT_TILES 18
#define BACKGROUND_WIDTH_PIXELS 160
#define BACKGROUND_HEIGHT_PIXELS 144

// Friendly name that corresponds with tile position in sprite sheet(s). 
enum { TILE_BALL = 0, TILE_WALL = 1, TILE_PIN = 2 };

// === SPRITE HANDLING ===

// Allocates a sprite at sprite_index with a tile at tile_index
// Returns GameSprite struct that stores these values, plus other useful graphics data TBC
GameSprite create_sprite(uint8_t tile_index);

// === BACKGROUNDS ===

// Loads background tiles and map into memory
void set_game_background(unsigned char *background, unsigned char *tiles);

unsigned char* get_game_background_tilemap(void);

// Load in a sprite sheet
void set_sprite_sheet(unsigned char *sprite_sheet);

// === DRAW FUNCTIONS ===

// Fast sprite draw using GBDK in move_sprite routine. Pass pointer to GameSprite
// Offsets are added here because Game Boy sprites are rendered at (x+8, y+16)
#define DRAW_SPRITE(obj_ptr, x, y) \
    move_sprite((obj_ptr)->sprite_index, x + 8, y + 16)

// Plot a point at position in pixel space (int)
void plot_point_fixed(fixed_n x, fixed_n y);

// === ANIMATIONS ===
// Future-proofing for animation functions

#endif // GRAPHICS_H_
// main.c

#include <gb/gb.h>
#include "game_data.h"
#include "state_manager.h"

void main(void) 
{
    set_state(STATE_GAME_SCREEN); 

    while (1) {

        update_state();

    }
}

/* End of main.c */
// physics.c

#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"

void update_ball_position(GameObject* ball)
{
    ball->physics.vy += GRAVITY;

    if (ball->physics.vy > MAX_SPEED)
        ball->physics.vy = MAX_SPEED;
    
    // Add our velocity to the fixed number fraction left over from last frame
    ball->physics.fractional_vx += ball->physics.vx;
    ball->physics.fractional_vy += ball->physics.vy;

    // Convert fixed value back to signed integer and apply to screen position
    ball->transform.x += (int8_t)(ball->physics.fractional_vx >> FIXED_SHIFT);
    ball->transform.y += (int8_t)(ball->physics.fractional_vy >> FIXED_SHIFT);
    
    // Zero off the left-hand byte to leave us with the decimal
    ball->physics.fractional_vx &= 0xFF;
    ball->physics.fractional_vx &= 0xFF;

}

void apply_impulse(GameObject* obj, fixed_n impulse_magnitude_x, fixed_n impulse_magnitude_y)
{
    obj->physics.vx += impulse_magnitude_x;
    obj->physics.vy += impulse_magnitude_y;
}

/**
 * @brief Checks a Ball for any collisions with pins, if so applies a bounce
 *  @todo Order of checks could be optimised
 * Change the pins so they are at the top of the tile...
 * My god this likely needs optimising the hell out of.
 * Although filters work well, unlikely to be lots of balls in actual collision simultaeneousy
 * @param ball The Ball object reference to check 
 */
void handle_ball_pin_collision(GameObject* ball)
{
    
    // FILTER: Only handle collision if ball is moving downward
    if (ball->physics.vy <= 0)
        return;

    // Get ball's bottom-middle point
    uint8_t ball_bottom_x = ball->transform.x + TILE_HALF_WIDTH;
    uint8_t ball_bottom_y = ball->transform.x  + TILE_WIDTH;

    // Find which 8x8 tile the ball's bottom is in
    uint8_t col = PIXEL_TO_GRID(ball_bottom_x);
    uint8_t row = PIXEL_TO_GRID(ball_bottom_y);

    // FILTER: Outside screen, do not collide
    if (col >= BACKGROUND_WIDTH_TILES || row >= BACKGROUND_HEIGHT_TILES)
        return;

    // Grab the tiles index value
    uint16_t tilemap_index = GET_TILE_INDEX(col, row);

    // FILTER: Check the tile in the map at that index, if it's a pin tile we need to collide
    if (get_game_background_tilemap()[tilemap_index] != PIN_TILE_ID)
        return;
            
    // Get coords of pin tile
    uint8_t pin_tile_x = GRID_TO_PIXEL(col);
    uint8_t pin_tile_y = GRID_TO_PIXEL(row);

    // Calculate horizontal distances to determine if ball will hit pin
    uint8_t pin_center_x = pin_tile_x + TILE_HALF_WIDTH;
    int8_t distance_x = ball_bottom_x - pin_center_x;
    
    if (distance_x < -(PIN_HALF_WIDTH) || distance_x > (PIN_HALF_WIDTH))
        return;

    // Calculate vertical distance so collision only happens when top of pin is hit
    uint8_t pin_visual_top = pin_tile_y + PIN_HALF_WIDTH;
    int8_t vertical_dist = ball_bottom_y - pin_visual_top;
    
    // Distance for collision is 0 - 4, so anything above 4 or less than 0 SKIP
    if (vertical_dist < 0 || vertical_dist > TILE_HALF_WIDTH)
        return;

    // Zero fractional velocity
    ball->physics.fractional_vy = 0;

    if (ball->physics.vy > FIXED_QUARTER) { 
        // === BOUNCE ===
        
        // Vertical bounce (50% energy)
        ball->physics.vy = -(ball->physics.vy >> 1);
        
        // If ball moving right and hits left side (distance_x < 0), bounce left
        // If ball moving left and hits right side (distance_x > 0), bounce right
        if ((ball->physics.vx > 0 && distance_x < 0) || (ball->physics.vx < 0 && distance_x > 0)) {
            // Hit the "wrong" side - reverse direction with damping
            ball->physics.vx = -(ball->physics.vx >> 1);
        } else {
            // Hit the "correct" side - keep direction but add deflection force
            ball->physics.vx = (ball->physics.vx >> 1);
            
            // Add a nudge based on offset from center (scaled down)
            if (distance_x > 0) {
                ball->physics.vx += FIXED_EIGHTH;  // nudge right
            } else if (distance_x < 0) {
                ball->physics.vx -= FIXED_EIGHTH;  // nudge left
            } else {
                // Dead center hit - give it a small nudge to break symmetry
                ball->physics.vx += (ball->transform.y & 1) ? FIXED_EIGHTH : -FIXED_EIGHTH;
            }
        }
        
    } else { 
        // === ROLL ===
        
        ball->physics.vy = 0;
        
        // Shift distance_x to scale it down
        ball->physics.vx += (distance_x >> 2);
        
        // Clamp horizontal speed
        if (ball->physics.vx > MAX_ROLL_SPEED) ball->physics.vx = MAX_ROLL_SPEED;
        else if (ball->physics.vx < -MAX_ROLL_SPEED) ball->physics.vx = -MAX_ROLL_SPEED;
    }
}

// Lookup tables

const fixed_n RANDOM_HORIZONTAL_VX[20] = {
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
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

/* End of physics.c */
/**
 * @file physics.h
 * @brief Contains physics functionality to the Game Objects.
 * * @details This contains functions for object movement and interactions.
 * The physics is not true to life but is instead optimised for speed.
 * As long as it looks good and it's cheap then that's fine.
 * We have constants that define certain forces like gravity and roll speed.
 * All physics interactions take place in "fixed-number space".
 * This means that the fixed number decimals are used for higher precision.
 * The update_position functions will convert the numbers back to integer and set x,y.
 * * @defgroup Physics
 */
#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "custom_types.h"
#include "game_object.h"

// Physics constants (8.8 fixed-point)
#define GRAVITY     FIXED_TEENTH  // 256/16
#define MAX_SPEED   TO_FIXED(2) // Max speed of balls in fixed-point number space
#define ROLL_FORCE FIXED_QUARTER // Amount of force applied to ball to continue roll
#define MAX_ROLL_SPEED TO_FIXED(1) // Max speed ball can go while rolling
#define HORIZONTAL_PIN_FORCE FIXED_QUARTER // For bounce force on x-axis

// === FORCE ===

// Apply instant impulse force to ball
void apply_impulse(GameObject* obj, fixed_n impulse_magnitude_x, fixed_n impulse_magnitude_y);

// === BALL PHYSICS ===

void update_ball_position(GameObject* obj);
void apply_impulse(GameObject* obj, fixed_n impulse_x, fixed_n impulse_y);
void handle_ball_pin_collision(GameObject* obj);

// === LOOKUP TABLES ===

// Random horizontal velocities for ball resets
extern const fixed_n RANDOM_HORIZONTAL_VX[20];

#endif // PHYSICS_H_
// pins.c

#include "pins.h"
#include "graphics.h" // Needs create_sprite, TILE_PIN

/* End of pins.c */
/**
 * @file pins.h
 * @brief Provides functionality for pin Game Objects.
 * * @details This module provides functions to initialise pins.
 * It can also provide functions for any pin related logic and actions.
 * * @defgroup Game Object
 */
#ifndef PINS_H_
#define PINS_H_

#include "custom_types.h"

#endif // PINS_H_
// state_manager.c

#include <gb/gb.h>

#include "state_manager.h"
#include "game_data.h"

// State headers
#include "states/state_title_screen.h"
#include "states/state_game_screen.h"


void set_state(uint8_t new_state)
{
    if (game.system.current_state != new_state) {

        game.system.current_state = new_state;

        init_state();
    }
}

void buffer_state(uint8_t state)
{
    // Load state data into a copy of init and game_data
}


void init_state(void)
{
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
            init_game_screen();
            break;
        case STATE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown state
            break;
    }
}

void update_state(void)
{
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
            update_game_screen();
            break;
        case STATE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown state
            break;
    }
    
}

void cleanup_state(void)
{
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
            // code
            break;
        case STATE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown state
            break;
    }
}

/* End of state_manager.c */
/**
 * @file state_manager.h
 * @brief Manages the high-level game state machine.
 * * @details This module provides the public interface for initializing, 
 * updating, and cleaning up the current game state (e.g., Title Screen, Game Screen).
 * It acts as a central switchboard for the main game loop logic.
 * * @defgroup StateModule Game State Management
 */

#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include "custom_types.h"

// === STATES ===

#define STATE_GAME_SCREEN 1
#define STATE_TITLE_SCREEN 2
#define STATE_SCORE_SCREEN 3
#define STATE_DEMO_SCREEN 4


void set_state(uint8_t state);

// Init state in memory, set_state will check if state is buffered and reference those values
// This may be used to reduce load times... !!! NOT IMPLEMENTED !!!
void buffer_state(uint8_t state);

void init_state(void);

void update_state(void);

void cleanup_state(void);

#endif // STATE_MANAGER_H_
/*

/maps/pachinkoMap.c

 Map Source File.

 Info:
   Section       : 
   Bank          : 0
   Map size      : 20 x 18
   Tile set      : game_pachinko_tiles.gbr
   Plane count   : 1 plane (8 bits)
   Plane order   : Tiles are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/

#define pachinko1Width 20
#define pachinko1Height 18
#define pachinko1Bank 0

unsigned char PachinkoMap[] =
{
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x02,0x0F,0x02,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x02,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x02,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x02,0x0F,0x0F,0x02,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x02,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x02,0x0F,0x0F,0x0F,0x02,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x02,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x02,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x02,
  0x0F,0x0F,0x0F,0x0F,0x02,0x0F,0x02,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x03,
  0x0F,0x0F,0x0F,0x0F,0x02,0x0F,0x0F,0x02,0x0F,0x02,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x03,0x05,
  0x06,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x03,0x05,0x04,
  0x07,0x06,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x02,0x0F,0x0F,0x0F,0x03,0x05,0x04,0x04,
  0x04,0x07,0x06,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x02,0x0F,0x0F,0x0F,0x03,0x05,0x04,0x04,0x04,
  0x04,0x04,0x07,0x06,0x0F,0x0F,0x0F,0x02,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x0F,0x03,0x05,0x04,0x04,0x04,0x04,
  0x04,0x04,0x04,0x07,0x06,0x0F,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x0F,0x03,0x05,0x04,0x04,0x04,0x04,0x04,
  0x04,0x04,0x04,0x04,0x07,0x06,0x0F,0x0F,0x0F,0x0F,
  0x0F,0x0F,0x03,0x05,0x04,0x04,0x04,0x04,0x04,0x04
};

/* End of /maps/pachinkoMap.c */
/*

 PACHINKOMAP.H

 Map Include File.

 Info:
   Section       : 
   Bank          : 0
   Map size      : 20 x 18
   Tile set      : game_pachinko_tiles.gbr
   Plane count   : 1 plane (8 bits)
   Plane order   : Tiles are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/

#define pachinko1Width 20
#define pachinko1Height 18
#define pachinko1Bank 0

extern unsigned char PachinkoMap[];

/* End of PACHINKOMAP.H */
// states/state_game_screen.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

#include "../game_data.h"
#include "../game_object.h"
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../pins.h"

#include "state_game_screen.h"

#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

void init_game_screen(void)
{
    DISPLAY_OFF;
    SPRITES_8x8;
    
    // Initialize the Object Manager (Resets counters)
    go_init_manager(); 
    
    // Reset the sprite counter
    game.graphics.next_sprite_id = 0;

    // Load in the sprite and background we want
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(PachinkoMap, PanchinkoTiles);

    // Spawn some balls in
    for (uint8_t i = 0; i < MAX_BALLS; i++) {

        GameObject* ball;

        // Set initial position based on index
        if (i < 8) {
            uint8_t ball_x = 10 + i*8;
            uint8_t ball_y = 20;
            ball = spawn_ball(ball_x, ball_y);
        } else {
            uint8_t ball_x  = 10 + (i-8)*8;
            uint8_t ball_y = 30;
            ball = spawn_ball(ball_x, ball_y);
        }

        // Give random horizontal speed
        ball->physics.vx = RANDOM_HORIZONTAL_VX[i];
    }

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}

void update_game_screen(void)
{
    game.system.previous_keys = game.system.keys;
    game.system.keys = joypad();

    // INPUT MISSING

    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_all_balls();
    }
    
    // Update all game objects
    // All ball update logic in associated source files. Check ball.c for ball update
    go_update_all_balls();

    // Draw all game objects
    go_draw_all_balls();
        
    vsync();
}

void cleanup_game_screen(void)
{
    HIDE_SPRITES; 
    HIDE_BKG;
}

/* End of states/state_game_screen.c */
// state_game_screen.h

#ifndef STATE_GAME_SCREEN_H_
#define STATE_GAME_SCREEN_H_

void init_game_screen(void);

void update_game_screen(void);

void cleanup_game_screen(void);

#endif // STATE_GAME_SCREEN_H_
// /states/state_title_screen.c

/* End of /states/state_title_screen.c */
// state_title_screen.h

#ifndef STATE_TITLE_SCREEN_H_
#define STATE_TITLE_SCREEN_H_


#endif // STATE_TITLE_SCREEN_H_
/*

 PINBALLTILES.C

 Tile Source File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : None.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 15

  Palette colors       : None.
  SGB Palette          : None.
  CGB Palette          : None.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/

/* Start of tile array. */
unsigned char PanchinkoTiles[] =
{
  0x00,0x00,0x3C,0x3C,0x76,0x78,0x7A,0x7E,
  0x7E,0x7E,0x7E,0x7E,0x3C,0x3C,0x00,0x00,
  0xFF,0xFF,0x00,0xFF,0x00,0xFF,0xFF,0x00,
  0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,
  0x00,0x3C,0x10,0x18,0x00,0x00,0x00,0x00,
  0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x0F,
  0x00,0x1F,0x00,0x3F,0x00,0x7F,0x00,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0x01,0xFF,0x03,0xFF,0x07,0xFF,0x0F,0xFF,
  0x1F,0xFF,0x3F,0xFF,0x7F,0xFF,0xFF,0xFF,
  0x00,0x80,0x00,0xC0,0x00,0xE0,0x00,0xF0,
  0x00,0xF8,0x00,0xFC,0x00,0xFE,0x00,0xFF,
  0x80,0xFF,0xC0,0xFF,0xE0,0xFF,0xF0,0xFF,
  0xF8,0xFF,0xFC,0xFF,0xFE,0xFF,0xFF,0xFF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/* End of /tiles/pachinkoTiles.C */
/*

 PINBALLTILES.H

 Include File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : None.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 15

  Palette colors       : None.
  SGB Palette          : None.
  CGB Palette          : None.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/


/* Bank of tiles. */
#define PinballTilesBank 0
/* Start of tile array. */
extern unsigned char PanchinkoTiles[];

/* End of PINBALLTILES.H */
