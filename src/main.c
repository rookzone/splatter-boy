//main.c

/*
TODO:

https://www.youtube.com/watch?v=oLoTTTuvaRs&list=PLeEj4c2zF7PaFv5MPYhNAkBGrkx4iPGJo&index=10
- collision detection



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
#include "graphics.h"

// Create game object structs
Ball pinball;
Wall floor;
Ball pachinkoBalls[10];
GameSprite pachinko_balls_gfx_data[10];


void main(void) 
{
    DISPLAY_OFF;
    SPRITES_8x8;

    // Load in sprite sheet
    set_sprite_data(0, 2, PinballTiles);


    // Initialize standalone ball
    pinball.x = TO_FIXED(95);
    pinball.y = TO_FIXED(75);
    pinball.vx = TO_FIXED(0);
    pinball.vy = TO_FIXED(0);

    GameSprite pinball_graphics_data;
    pinball_graphics_data = create_sprite(TILE_BALL);
    pinball.game_sprite = &pinball_graphics_data;

    DRAW_SPRITE(pinball.game_sprite,pinball.x,pinball.y);
  

    // Initialize floor
    floor.x = 0;
    floor.y = 120;
    floor.width = 160;
    floor.height = 8;

    GameSprite wall_graphics_data;
    wall_graphics_data = create_sprite(TILE_WALL);
    floor.game_sprite = &wall_graphics_data;

    DRAW_SPRITE(floor.game_sprite,TO_FIXED(floor.x),TO_FIXED(floor.y));

    // Create the pachinko balls, all handled in this function
    init_balls(pachinkoBalls, pachinko_balls_gfx_data, 10);

    DISPLAY_ON;
    SHOW_SPRITES;

    uint8_t frame_time;
    printf("Frame time:   \n");
    printf("Draw posit:   \n");
    bool frame_advance_mode = false; 
    uint8_t keys;

    while (1) {
      
      
      // INPUT
      if (joypad() == J_RIGHT && frame_advance_mode == false){
        frame_advance_mode = true;
      }

      if (joypad() & J_DOWN) {
        reset_balls(pachinkoBalls, 10);
      }
      
      /* ##### START OF PERFORMANCE MEASURE BLOCK ##### */
      uint8_t start = DIV_REG;

      // PHYSICS
      apply_gravity(&pinball);
      check_ball_wall(&pinball, &floor);

      apply_gravity_multi(pachinkoBalls, 10);
      check_ball_wall_multi(pachinkoBalls, &floor, 10);
      
      if (joypad() & J_UP) {
        apply_impulse(&pinball, 50);
      }

      // DRAW SPRITE IN UPDATED POSITIONS
      for (uint8_t i = 0; i < 10; i++) {
        DRAW_SPRITE(pachinkoBalls[i].game_sprite,pachinkoBalls[i].x,pachinkoBalls[i].y);
      }

      DRAW_SPRITE(pinball.game_sprite,pinball.x,pinball.y);

      uint8_t end = DIV_REG;
      /* ##### END OF PERFORMANCE MEASURE BLOCK ##### */

      frame_time = end - start;

      gotoxy(12, 0);
      printf("%3u", frame_time);


      // This section just handles a manual frame advance mode. Can be removed without issue.
      // I'm aware that this could be done much more cleanly, but I like the goto bodge job for aesthetic reasons.
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


