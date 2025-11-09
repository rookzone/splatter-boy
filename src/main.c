
#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>
#include "tiles/pinballTiles.h"


/*########## TODO #############
- Render wall sprites
- Log collision events
- Detect if ball is rolling or at rest on surface
  - Defined if collision event is the same after each frame, then we can assume that the ball is in a continous collision state
  - This meanswe can apply some friction or allow the wall to exert force on the ball
###########*/

enum {
    PLAYER_SPRITE = 0,
    BALL_SPRITE = 0,
    WALL_SPRITE = 1
} Sprites;

enum {
    TILE_SMILE = 0,
    TILE_BALL = 0,
    TILE_WALL = 1
} Tiles;

// Ball struct
typedef struct {
    uint8_t x, y;   // position in fixed-point
    int8_t vx, vy; // velocity in fixed-point
} Ball;


typedef struct {
    uint8_t x, y;       // top-left position in pixels
    uint8_t width, height;
} Wall;


#define GRAVITY 1
#define MAX_SPEED 10
#define DAMPING 0

void apply_gravity(Ball *b);
void check_ball_wall(Ball *b, Wall *w);

void main(void) {

    DISPLAY_OFF;     
    SHOW_BKG;         
    SPRITES_8x8;      
	
    // Load tile data
	  set_sprite_data(0, 2, PinballTiles);
    
    // Create ball
    Ball pinball;
    pinball.x = 95;
    pinball.y = 75;
    pinball.vy = 0; // start stationary
    // Draw ball
    set_sprite_tile(BALL_SPRITE, TILE_BALL); 
    move_sprite(BALL_SPRITE, 95, 75);

    // Create floor wall
    Wall floor;
    floor.x = 0;
    floor.y = 120;     // near bottom of screen
    floor.width = 160; // full screen width
    floor.height = 8;  // 8 pixels tall
    set_sprite_tile(WALL_SPRITE, TILE_WALL); 
    move_sprite(WALL_SPRITE, 105, 75);

    printf("Testing a GameBoy \ngame.");

    DISPLAY_ON;
	  SHOW_SPRITES;

	while(1){

      // Apply gravity
      apply_gravity(&pinball);

      check_ball_wall(&pinball, &floor);
      
      // Update sprite position
      move_sprite(BALL_SPRITE, pinball.x, pinball.y);

      wait_vbl_done();
	}

}

void apply_gravity(Ball *b) {
  
  if (abs(b->vy) < MAX_SPEED){
    b->vy += GRAVITY;  // accelerate downward
  }
  b->y += b->vy;     // move down by velocity

}

void check_ball_wall(Ball *b, Wall *w) {
    if (b->y + 8 >= w->y) { // 8 = sprite height
        b->y = w->y - 8;
        b->vy = -b->vy / DAMPING;
    }
}  