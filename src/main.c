//main.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdbool.h>

#include "physics.h"
#include "ball.h"
#include "pins.h"
#include "graphics.h"
#include "debug.h"

#include "tiles/pinballTiles.h"
#include "tiles/pachinkoOneBG.h"

// Forward declarations
void init_game_state(void);
void game_state_input(void);
void game_state_physics(void);
void change_state(uint8_t state);
void end_step(void);

// Ball values
#define NUM_BALLS 16
#define NUM_PINS 36

// === RUNTIME GAME OBJECT DATA ===
Wall floor;
Ball pachinko_balls[NUM_BALLS];
Pin pachinko_pins[NUM_PINS];
Pin test_pin;

// === RUNTIME GRAPHICS DATA ===
GameSprite pachinko_balls_gfx_data[NUM_BALLS];
GameSprite pachinko_pins_gfx_data[NUM_PINS];
GameSprite wall_graphics_data;
GameSprite test_pin_graphics_data;

// === GAME (and/or) STATE ===

uint8_t current_state = 0;
uint8_t frame_counter = 0;

uint8_t keys; // current key press
uint8_t previous_keys; // Key on last frame

void main(void) 
{

  change_state(STATE_GAME_SCREEN);

  while (1) {

    switch(current_state) {

      case STATE_GAME_SCREEN:
        game_state_input();
        game_state_physics();
        end_step();
        break;
      
      case STATE_TITLE_SCREEN:
        break;

      case STATE_DEMO_SCREEN:
        break;

      case STATE_SCORE_SCREEN:
        break;

    }
  }
} // end of main

void change_state(uint8_t state)
{
  if (state != current_state){

    current_state = state;

    switch(current_state)
    {
      case STATE_GAME_SCREEN:
        init_game_state();
        current_state = state;
        // Main game screen
        break;

      case STATE_DEMO_SCREEN:
        // Demo screen
        break;

      case STATE_SCORE_SCREEN:
        // Score board
        break;

      case STATE_TITLE_SCREEN:
        // Title screen
        break;
    }
  }
}

// === GAME STATE FUNCTIONS ===

void init_game_state(void)
{
  DISPLAY_OFF;
  SPRITES_8x8;
  HIDE_BKG;
  HIDE_SPRITES;

  next_sprite_id = 0; // reset sprites
  set_sprite_data(0, 5, PinballTiles);

  // Load background
  set_game_background(pachinko1,PinballTiles);

  // Create two rows of balls
  for (uint8_t i = 0; i < NUM_BALLS/2; i++) {
    uint8_t x = 10 + i*8;
    uint8_t y = 20;
    init_ball(&pachinko_balls[i], &pachinko_balls_gfx_data[i], x, y);
    pachinko_balls[i].vx = RANDOM_HORIZONTAL_VX[i];

  }

  for (uint8_t i = 8; i < NUM_BALLS; i++) {
    uint8_t x = 10 + i*8;
    uint8_t y = 30;
    init_ball(&pachinko_balls[i], &pachinko_balls_gfx_data[i], x, y);
    pachinko_balls[i].vx = RANDOM_HORIZONTAL_VX[i];

  }

  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;
}

void game_state_input(void)
{
    
    keys = joypad();

    if ((keys & J_LEFT) && !(previous_keys & J_LEFT)) {

        Ball* ball_to_launch = find_lowest_ball(pachinko_balls, NUM_BALLS);

        if (ball_to_launch != NULL) {
            launch_ball(ball_to_launch, 0, 90, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y+FIXED_HALF);
        }
    }

    if ((keys & J_UP) && !(previous_keys & J_UP)) {
        reset_balls(pachinko_balls, NUM_BALLS);
    }
}

void game_state_physics(void)
{

  // Use the ball objects location to determine if it is on a pin tile for collision.
  // If on a pin tile then create a "virtual pin" at that tile and pass it
  for (uint8_t i = 0; i < NUM_BALLS; i++) {
        
    // Turn this into a function "check_collision" that is able to return the
    // index for the type of object collided into
    // then a switch case can be used to handle the various types of collisions
    // such as with a pin, a wall, a ball, a spinner, flipper etc etc.
    uint8_t ball_center_x = pachinko_balls[i].x + TILE_HALF_WIDTH;
    uint8_t ball_center_y = pachinko_balls[i].y + TILE_HALF_WIDTH;

    uint8_t col = PIXEL_TO_GRID(ball_center_x);
    uint8_t row = PIXEL_TO_GRID(ball_center_y);

    uint16_t tilemap_index = GET_TILE_INDEX(col, row);

    if (active_background_tilemap[tilemap_index] == PIN_TILE_ID) {
        
        //
        Pin virtual_pin;
        virtual_pin.x = GRID_TO_PIXEL(col);
        virtual_pin.y = GRID_TO_PIXEL(row);

        //
        handle_ball_pin_collision(&pachinko_balls[i], &virtual_pin);
    }
  
    //check_ball_wall(&pachinkoBalls[i], &floor);
    update_ball_position(&pachinko_balls[i]);
    
    DRAW_SPRITE(pachinko_balls[i].game_sprite, pachinko_balls[i].x, pachinko_balls[i].y);

  }
}

void end_step(void)
{
  previous_keys = keys; // Place value of keys into previous_keys for next frame
  vsync();
}

// end main.c