// ball.c
#include <gb/gb.h>
#include "ball.h"

void init_balls(Ball* b, GameSprite* gfx_data, uint8_t count)
{

    for(uint8_t i = 0; i < count; i++){
      b[i].x = TO_FIXED(60+ i*10);
      b[i].y = TO_FIXED(40+ i*10);
      b[i].vx = 0;
      b[i].vy = 0;

      gfx_data[i] = create_sprite(TILE_BALL);
      b[i].game_sprite = &gfx_data[i];
      move_sprite_fixed(b[i].game_sprite,b[i].x, b[i].y);

    }
}

void reset_balls(Ball* b, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++){
      b[i].x = TO_FIXED(60+ i*10);
      b[i].y = TO_FIXED(40+ i*10);
      b[i].vx = 0;
      b[i].vy = 0;
      move_sprite_fixed(b[i].game_sprite,b[i].x, b[i].y);
    }
}