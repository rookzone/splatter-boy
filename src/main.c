//main.c

/*
tODO:

 Add pins.c/h files

Define Pin struct

Implement init_pins() to place them visually

Store pins in an array

 Add simple overlap detection

Check if ball’s pixel position overlaps a pin

When true, flip ball->vy (and tweak ball->vx)

 Make an array of pin positions

Start small (3–4 pins)

Maybe later load this from a map file

 Integrate into main loop

Call check_pin_collisions() after check_ball_wall()
*/

#include <gb/gb.h>
#include <stdio.h>
#include "tiles/pinballTiles.h"
#include "physics.h"

enum { BALL_SPRITE = 0, WALL_SPRITE = 1 };
enum { TILE_BALL = 0, TILE_WALL = 1 };

Ball pinball;
Wall floor;

void main(void) {
    DISPLAY_OFF;
    SPRITES_8x8;

    set_sprite_data(0, 2, PinballTiles);

    // Initialize ball position and velocity
    pinball.x = FIXED(95);
    pinball.y = FIXED(75);
    pinball.vx = 0;
    pinball.vy = 0;

    set_sprite_tile(BALL_SPRITE, TILE_BALL);
    move_sprite(BALL_SPRITE, pinball.x >> 8, pinball.y >> 8);

    // Initialize floor
    floor.x = 0;
    floor.y = 120;
    floor.width = 160;
    floor.height = 8;

    set_sprite_tile(WALL_SPRITE, TILE_WALL);
    move_sprite(WALL_SPRITE, 95, floor.y - 1);

    DISPLAY_ON;
    SHOW_SPRITES;

    while (1) {
        apply_gravity(&pinball);
        check_ball_wall(&pinball, &floor);

        move_sprite(BALL_SPRITE, pinball.x >> 8, pinball.y >> 8);

        if (joypad() & J_UP) {
            apply_impulse(&pinball, 50);
        }

        wait_vbl_done();
    }
}
