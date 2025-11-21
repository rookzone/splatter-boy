// ball.c
#include "ball.h"

void reset_balls(Ball* b, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++){
      b[i].x = 10+ i*10;
      b[i].y = 10+ i*10;
      b[i].vx = 0;
      b[i].vy = 0;

      DRAW_SPRITE(b[i].game_sprite,b[i].x,b[i].y);

      b[i].vx = RANDOM_HORIZONTAL_VX[i];
      
    }
}

void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y)
{
    ball->x = ball_x;
    ball->y = ball_y;
    ball->vx = 0;
    ball->vy = 0;
    ball->sub_x = 0;
    ball->sub_y = 0;
    
    *gfx_data = create_sprite(TILE_BALL);
    ball->game_sprite = gfx_data;
}

void reset_ball(Ball* ball)
{
    ball->vx = 0;
    ball->vy = 0;
    ball->sub_x = 0;
    ball->sub_y = 0;
}