// ball.c
#include <gb/gb.h>
#include "ball.h"

void init_balls(Ball* b, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++){
      b[i].x = TO_FIXED(60+ i*10);
      b[i].y = TO_FIXED(40+ i*10);
      b[i].vx = 0;
      b[i].vy = 0;

      set_sprite_tile(2 +i, TILE_BALL);
      move_sprite(2 + i,TO_FIXED(b[i].x), TO_FIXED(b[i].y));
    }
}

void reset_balls(Ball* b, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++){
      b[i].x = TO_FIXED(60+ i*10);
      b[i].y = TO_FIXED(40+ i*10);
      b[i].vx = 0;
      b[i].vy = 0;
      move_sprite(2 + i,TO_FIXED(b[i].x), TO_FIXED(b[i].y));
    }
}