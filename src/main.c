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
#include "tiles/testLevelBG.h"

// Forward declarations
void init_game_state(void);
void game_state_input(void);
void game_state_physics(void);
void change_state(uint8_t state);
void end_step(void);

// Ball values
#define BALLS_SIZE 6
#define NUM_BALLS 6
#define NUM_PINS 10

// === RUNTIME GAME OBJECT DATA ===
Wall floor;
Ball pachinkoBalls[BALLS_SIZE];
Pin pachinkoPins[NUM_PINS];
Pin test_pin;

// === RUNTIME GRAPHICS DATA ===
GameSprite pachinko_balls_gfx_data[BALLS_SIZE];
GameSprite pachinko_pins_gfx_data[NUM_PINS];
GameSprite wall_graphics_data;
GameSprite test_pin_graphics_data;

// === GAME (and/or) STATE ===

uint8_t current_state = 0;

uint8_t frame_counter = 0;

// current key press etc
uint8_t keys;
// Key on last frame
uint8_t previous_keys;


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

}

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

  set_sprite_data(0, 3, PinballTiles);

  // Initialize floor
  floor.x = 0;
  floor.y = 144;

  wall_graphics_data = create_sprite(TILE_WALL);
  floor.game_sprite = &wall_graphics_data;


  // create a bunch of pins
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    uint8_t x = 40+ i*15;
    uint8_t y = 100;
    init_pin(&pachinkoPins[i], &pachinko_pins_gfx_data[i], x, y);
    DRAW_SPRITE(pachinkoPins[i].game_sprite, x, y);
  }


  // Create pachinko balls
  for (uint8_t i = 0; i < BALLS_SIZE; i++) {

    uint8_t x = 42+ i*12;
    uint8_t y = 20;
    init_ball(&pachinkoBalls[i], &pachinko_balls_gfx_data[i], x, y);
    pachinkoBalls[i].vx = RANDOM_HORIZONTAL_VX[i];

  }

  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;

}


void game_state_input(void)
{
    
    keys = joypad();

    
    if ((keys & J_LEFT) && !(previous_keys & J_LEFT)) {
        
        Ball* ball_to_launch = find_lowest_ball(pachinkoBalls, NUM_BALLS);
        
        if (ball_to_launch != NULL) {

            launch_ball(ball_to_launch, 0, 90, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y+FIXED_HALF);

        }
    }

    if ((keys & J_UP) && !(previous_keys & J_UP)) {
        reset_balls(pachinkoBalls, NUM_BALLS);
    }
    
}

void game_state_physics(void)
{
  // ** Tweak the divisor (e.g., 2, 3, or 4) to find the right balance **
  bool run_collision_check = (frame_counter % 2) == 0;
  frame_counter++; // Increment the counter every frame

  for (uint8_t i = 0; i < NUM_BALLS; i++) {
    
    if (run_collision_check) { // Only run this block every 2nd frame
        for (uint8_t j = 0; j < NUM_PINS; j++) {
            handle_ball_pin_collision(&pachinkoBalls[i], &pachinkoPins[j]);
        }
    }
    
    check_ball_wall(&pachinkoBalls[i], &floor);
    update_ball_position(&pachinkoBalls[i]);
    
    DRAW_SPRITE(pachinkoBalls[i].game_sprite, pachinkoBalls[i].x, pachinkoBalls[i].y);

  }
}

void end_step(void)
{
  previous_keys = keys;
  vsync();
}