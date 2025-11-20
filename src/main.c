//main.c

/*
# TODO

https://www.youtube.com/watch?v=oLoTTTuvaRs&list=PLeEj4c2zF7PaFv5MPYhNAkBGrkx4iPGJo&index=10
- collision detection

Start to document progress on a wiki.


## Structure

All structs, enum, constants, and macros are place in `CustomTypes.h`. This lets me define all the custom 
stuff in one place, reducing the amount of includes in header files and circular dependcies.

**These are the "engine" functions**

- Physics .c .h
Applying any movement and forces to logical positions and velocities.
Generally physics will be applied to the x,y,vx,vy values in the game objects (such as ball or pin).

Operations are done using 8.8 fixed decimal int16s, these helps increase precision.
These operations take placed in "fixed number space". They will need shifting back for drawing

- Graphics .c .h

**These are game object behaviours**

- ball .c .h
- pins .c .h

`main.c` is where the game loop is

*/

#include <gb/gb.h>
#include <gbdk/console.h>

#include <stdio.h>
#include <stdbool.h>

#include "tiles/pinballTiles.h"
#include "tiles/pachinkoOneBG.h"
#include "physics.h"
#include "ball.h"
#include "pins.h"
#include "graphics.h"
#include "debug.h"


#define BALLS_SIZE 10
#define NUM_BALLS 10

// === RUNTIME GAME OBJECT DATA ===
Wall floor;
Ball pachinkoBalls[BALLS_SIZE];
Pin test_pin;

// === RUNTIME GRAPHICS DATA ===
GameSprite pachinko_balls_gfx_data[BALLS_SIZE];
GameSprite wall_graphics_data;
GameSprite test_pin_graphics_data;

// === GAME (and/or) STATE ===

uint8_t current_state = STATE_GAME_SCREEN;

// current key press etc
uint8_t keys;

// Forward declarations

void game_state_input();
void end_step();
void change_state(uint8_t state);
void game_state_physics(uint8_t frame_skip);


void main(void) 
{
  DISPLAY_OFF;
  SPRITES_8x8;
  HIDE_BKG;
  HIDE_SPRITES;

  switch (current_state) {
    
    case STATE_DEMO_SCREEN:
      // code block
      break;

    case STATE_GAME_SCREEN:
      init_game_state();
      break;

    case STATE_SCORE_SCREEN:
      // code
      break;

    case STATE_TITLE_SCREEN:
      // code
      break;
  }

  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;

  while (1) {


    while(current_state = STATE_GAME_SCREEN)
    {

      // Checks for inputs and set behaviour and flags
      game_state_input();

      // Updates physics, location, and renders
      game_state_physics(0);

      // Calls vsync and other end of frame code
      end_step();

    }

  }

}

void end_step(){
  vsync();
}

void change_state(uint8_t state)
{
  if (state != current_state){
    current_state = state;
  }

}

// ##### GAME STATE FUNCTIONS #####

void init_game_state()
{
    
  set_sprite_data(0, 3, PinballTiles);

  // Initialize floor
  floor.x = 0;
  floor.y = 144;

  wall_graphics_data = create_sprite(TILE_WALL);
  floor.game_sprite = &wall_graphics_data;

  // init test pin
  init_pin(&test_pin, &test_pin_graphics_data, 60, 60);
  DRAW_SPRITE(test_pin.game_sprite, test_pin.x, test_pin.x);


  // Create pachinko balls
  for (uint8_t i = 0; i < BALLS_SIZE; i++) {

    uint8_t x = 10+ i*10;
    uint8_t y = 10+ i*10;
    init_ball(&pachinkoBalls[i], &pachinko_balls_gfx_data[i], x, y);

  }

}

void game_state_input()
{

  keys = joypad();

  // replace with switch case if more conditions
  if (keys == J_DOWN) {
    reset_balls(pachinkoBalls, 10);
  }

}

void game_state_physics(uint8_t frame_skip)
{
    // Handle balls and pins main loop. Per frame.
    // Note: Could be factored into seperate function with parameter for frame-skipping for performance.
    for (uint8_t i = 0; i < 10; i++) {

      // === PHYSICS ===

      update_ball_position(&pachinkoBalls[i]); // Apply continous gravity force of GRAVITY
      check_ball_wall(&pachinkoBalls[i], &floor); // Handle collisions with walls
      handle_ball_pin_collision(&pachinkoBalls[i], &test_pin); // Handle collisions with pins

      // === GRAPHICS ===
      DRAW_SPRITE(pachinkoBalls[i].game_sprite, pachinkoBalls[i].x, pachinkoBalls[i].y);
      
    }
}



