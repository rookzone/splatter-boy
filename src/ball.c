// ball.c

#include "ball.h"
#include "memory.h" // Now includes memory for full context access

// Forward declarations for functions moved out of main.c
extern const fixed_n RANDOM_HORIZONTAL_VX[];

void reset_balls(Ball* b, uint8_t count)
{
    // The previous logic was complex; I'll simplify it to use the new memory context
    // This function can be called from game_state_init

    for (uint8_t i = 0; i < count/2; i++) {
        b[i].x = 10 + i*8;
        b[i].y = 20;
        b[i].vx = 0;
        b[i].vy = 0;

        // DRAW_SPRITE uses the GameSprite pointer which is set in init_ball
        // The draw routine in states.c handles drawing in the loop
        
        b[i].vx = RANDOM_HORIZONTAL_VX[i];
    }
    
    for (uint8_t i = 8; i < count; i++) {
        b[i].x = 20 + i*8;
        b[i].y = 30;
        b[i].vx = 0;
        b[i].vy = 0;

        // DRAW_SPRITE uses the GameSprite pointer which is set in init_ball
        // The draw routine in states.c handles drawing in the loop

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


Ball* find_lowest_ball(Ball* balls, uint8_t count)
{
    if (count == 0) {
        return NULL; // Return NULL if the array is empty
    }

    Ball* lowest_ball = &balls[0];
    
    // Find the ball with the highest Y coordinate (lowest on the screen)
    for (uint8_t i = 1; i < count; i++) {
        if (balls[i].y > lowest_ball->y) {
            lowest_ball = &balls[i];
        }
    }
    return lowest_ball;
}

void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y)
{
    ball->x = from_x;
    ball->y = from_y;
    apply_impulse(ball, launch_power_x, launch_power_y);
}