// ball.c

#include "ball.h"

void reset_balls(Ball* b, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++){
      b[i].x = 20 + i*8;
      b[i].y = 20;
      b[i].vx = 0;
      b[i].vy = 0;

      DRAW_SPRITE(b[i].game_sprite,b[i].x,b[i].y);

      b[i].vx = RANDOM_HORIZONTAL_VX[(count-1)-i];

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


Ball* find_lowest_ball(Ball* balls, uint8_t count)
{
    if (count == 0) {
        return NULL; // Return NULL if the array is empty
    }

    Ball* lowest_ball = &balls[0];
    uint8_t highest_y = balls[0].y;

    for (uint8_t i = 1; i < count; i++) {
        // Compare the current ball's y-coordinate with the maximum found so far
        if (balls[i].y > highest_y) {
            highest_y = balls[i].y;
            lowest_ball = &balls[i];
        }
    }

    return lowest_ball;
}



void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y)
{
    // Reset ball velocity and sub-pixel accumulator
    reset_ball(ball);

    // Move ball to screen position
    ball->x = from_x;
    ball->y = from_y;

    apply_impulse(ball, launch_power_x, launch_power_y);

}