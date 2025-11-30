//main.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

#include "physics.h"
#include "ball.h"
#include "pins.h"
#include "graphics.h"
#include "gamestate.h"

// Tiles
#include "tiles/pinballTiles.h"
#include "tiles/pachinkoOneBG.h"

// Forward declarations
void init_game_state(void);
void game_state_input(void);
void game_state_physics(void);
void change_state(uint8_t state);
void end_step(void);

// === RUNTIME GAME OBJECT DATA ===
Pin test_pin;

// === RUNTIME GRAPHICS DATA ===
GameSprite test_pin_graphics_data;

void main(void) 
{

  change_state(STATE_GAME_SCREEN);

  while (1) {

    switch(game.system.current_state) {

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
  if (state != game.system.current_state){

    game.system.current_state = state;

    switch(game.system.current_state)
    {
      case STATE_GAME_SCREEN:
        init_game_state();
        game.system.current_state = state;
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

  game.graphics.next_sprite_id = 0; // reset sprites

  set_sprite_sheet(PinballTiles);

  // Load background
  set_game_background(pachinko1, PinballTiles);

  // Create two rows of balls
  for (uint8_t i = 0; i < NUM_BALLS/2; i++) {
    uint8_t x = 10 + i*8;
    uint8_t y = 20;
    init_ball(&game.objects.balls.list[i], &game.objects.balls.graphics[i], x, y);
    game.objects.balls.list[i].vx = RANDOM_HORIZONTAL_VX[i];

  }

  for (uint8_t i = 8; i < NUM_BALLS; i++) {
    uint8_t x = 10 + i*8;
    uint8_t y = 30;
    init_ball(&game.objects.balls.list[i], &game.objects.balls.graphics[i], x, y);
    game.objects.balls.list[i].vx = RANDOM_HORIZONTAL_VX[i];

  }

  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;
}

void game_state_input(void)
{
    
    game.system.keys = joypad();

    if ((game.system.keys & J_LEFT) && !(game.system.previous_keys & J_LEFT)) {

        Ball* ball_to_launch = find_lowest_ball(game.objects.balls.list, NUM_BALLS);

        if (ball_to_launch != NULL) {
            launch_ball(ball_to_launch, 0, 90, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y+FIXED_HALF);
        }
    }

    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_balls(game.objects.balls.list, NUM_BALLS);
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
    uint8_t ball_center_x = game.objects.balls.list[i].x + TILE_HALF_WIDTH;
    uint8_t ball_center_y = game.objects.balls.list[i].y + TILE_HALF_WIDTH;

    uint8_t col = PIXEL_TO_GRID(ball_center_x);
    uint8_t row = PIXEL_TO_GRID(ball_center_y);

    uint16_t tilemap_index = GET_TILE_INDEX(col, row);

    if (game.graphics.active_background_tilemap[tilemap_index] == PIN_TILE_ID) {
        
        // game.
        Pin virtual_pin;
        virtual_pin.x = GRID_TO_PIXEL(col);
        virtual_pin.y = GRID_TO_PIXEL(row);

        //
        handle_ball_pin_collision(&game.objects.balls.list[i], &virtual_pin);
    }
  
    update_ball_position(&game.objects.balls.list[i]);
    
    DRAW_SPRITE(game.objects.balls.list[i].game_sprite, game.objects.balls.list[i].x, game.objects.balls.list[i].y);

  }
}

void end_step(void)
{
  game.system.previous_keys = game.system.keys; // Place value of keys into previous_keys for next frame
  vsync();
}

// end main.c