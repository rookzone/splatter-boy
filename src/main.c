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

void main(void) 
{
  DISPLAY_OFF;
  SPRITES_8x8;
  HIDE_BKG;
  HIDE_SPRITES;

  set_sprite_data(0, 3, PinballTiles);

  // load background tiles into VRAM
  //set_bkg_data(0, 4, PinballTiles);
  //set_bkg_tiles(0, 0, 20, 18, pachinko1);


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
  

  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;

  bool frame_advance_mode = false; 
  uint8_t keys;
  
  while (1) {

    // === INPUT ===
    if (joypad() == J_RIGHT && frame_advance_mode == false){
      frame_advance_mode = true;
    }

    if (joypad() & J_DOWN) {
      reset_balls(pachinkoBalls, 10);
    }

    // Handle balls and pins main loop. Per frame.
    // Note: Could be factored into seperate function with parameter for frame-skipping for performance.
    for (uint8_t i = 0; i < 10; i++) {

      // === PHYSICS ===

      apply_gravity(&pachinkoBalls[i]); // Apply continous gravity force of GRAVITY
      check_ball_wall(&pachinkoBalls[i], &floor); // Handle collisions with walls
      handle_ball_pin_collision(&pachinkoBalls[i], &test_pin); // Handle collisions with pins
      
      //apply_impulse(&pachinkoBalls[i],RANDOM_HORIZONTAL_VX[i],0); // test horizontal impulse

      // === GRAPHICS ===
      DRAW_SPRITE(pachinkoBalls[i].game_sprite, pachinkoBalls[i].x, pachinkoBalls[i].y);
      
    }


    // This section just handles a manual frame advance mode. Can be removed without issue.
    // I'm aware that this could be done much mdore cleanly, but I like the goto bodge job for aesthetic reasons.
    loop:
    keys = joypad();
    if (keys == J_LEFT){
        frame_advance_mode = false;
        goto advance;
    }
    
    if(frame_advance_mode == true && keys != J_LEFT){
      if(keys != J_RIGHT)
        goto loop;
    }
    advance:

    vsync();

  }

}