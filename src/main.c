//main.c

/*
TODO:

Create helper function for assigning sprites. It should keep track of the sprite number.
I could also designate the sprite group, so if it's something that can be written over or if it's something
that needs to stay on the screen.

---

I need to think of some way of updating sprite positions and graphics too. I have these things to all associate together:
1. The struct
basically objects created that contain values

2. The physics - in fixed point numbers
The struct references are passed into this basically, then the numbers are changed in fixed point

3. Drawing the sprite
I am setting a sprite number for the object, and also a tile type. 

4. Moving the sprite
I am moving the sprite with pure int, so << 8 conversion.

---

Start to document progress on a wiki.

---

Next steps after this housekeeping:

- Create the pins, these are new objects where the balls will bounce off
requires a collision check
imparts force in direction depending on which pixel is hit, so hit at left side impart force leftwards on ball...

- Better ball motion
The balls will now need to be able to move left and right. So I'm guessing I'll need to look into vectors and how they work.

*/

#include <gb/gb.h>
#include <gbdk/console.h>

#include <stdio.h>
#include <stdbool.h>

#include "tiles/pinballTiles.h"
#include "physics.h"
#include "ball.h"

// Create game object structs
Ball pinball;
Wall floor;
Ball pachinkoBalls[10];


void main(void) 
{
    DISPLAY_OFF;
    SPRITES_8x8;

    // Load in sprite sheet
    set_sprite_data(0, 2, PinballTiles);

    // Initialize logical ball position and velocity
    pinball.x = TO_FIXED(95);
    pinball.y = TO_FIXED(75);
    pinball.vx = TO_FIXED(0);
    pinball.vy = TO_FIXED(0);

    // assign pinball a sprite, and move sprite to align with logical position
    set_sprite_tile(BALL_SPRITE, TILE_BALL);
    move_sprite(BALL_SPRITE, FROM_FIXED(pinball.x), FROM_FIXED(pinball.y));

    // Initialize floor
    floor.x = 0;
    floor.y = 120;
    floor.width = 160;
    floor.height = 8;

    set_sprite_tile(WALL_SPRITE, TILE_WALL);
    move_sprite(WALL_SPRITE, 95, floor.y - 1);

    init_balls(pachinkoBalls, 10);

    DISPLAY_ON;
    SHOW_SPRITES;

    uint8_t frame_time;
    printf("Frame time:   \n");
    bool frame_advance_mode = false; 
    uint8_t keys;

    while (1) {
      
      if (joypad() == J_RIGHT && frame_advance_mode == false){
        frame_advance_mode = true;
      }
      
      uint8_t start = DIV_REG; // Start of perfomence measure block

      apply_gravity(&pinball);
      check_ball_wall(&pinball, &floor);
      move_sprite(BALL_SPRITE, FROM_FIXED(pinball.x), FROM_FIXED(pinball.y));

      apply_gravity_multi(pachinkoBalls, 10);
      check_ball_wall_multi(pachinkoBalls, &floor, 10);
      
      if (joypad() & J_UP) {
        apply_impulse(&pinball, 50);
      }

      uint8_t end = DIV_REG; // end of performence measure block

      // Resets the balls into their initial position
      if (joypad() & J_DOWN) {
        reset_balls(pachinkoBalls, 10);
      }
      
      frame_time = end - start;

      gotoxy(12, 0);
      printf("%3u", frame_time);

      // This just
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

      wait_vbl_done();

    }
}


