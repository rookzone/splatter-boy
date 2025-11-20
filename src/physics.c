#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "graphics.h"


void update_ball_position(Ball *ball)
{

    apply_gravity(ball);
    
    ball->sub_x += ball->vx;
    ball->sub_y += ball->vy;
    
    int8_t dx = FROM_FIXED(ball->sub_x);  // >> 8
    int8_t dy = FROM_FIXED(ball->sub_y);  // >> 8
    
    ball->x += dx;
    ball->y += dy;
    
    ball->sub_x -= TO_FIXED(dx);  // - (dx << 8)
    ball->sub_y -= TO_FIXED(dy);  // - (dy << 8)

}

void apply_gravity(Ball *ball) 
{
    ball->vy += GRAVITY;
    
    if (ball->vy > MAX_SPEED) {
        ball->vy = MAX_SPEED;
    }

}

void apply_impulse(Ball *ball, fixed_n impulse_magnitude_x, fixed_n impulse_magnitude_y)
{

    ball->vx += impulse_magnitude_x;
    ball->vy += impulse_magnitude_y;

}

void check_ball_wall(Ball *ball, Wall *w) 
{
    if (ball->vy > 0) { 
        uint8_t sprite_y = ball->y + SPRITE_SIZE;

        if (sprite_y >= w->y + SPRITE_SIZE) {
            // Position correction
            ball->y = w->y;
            
            // Bounce with damping
            ball->vy = -(ball->vy >> 2);  // Divide by 4 and negate
            ball->vx = ball->vx >> 1;     // Halve horizontal velocity
            
            // Clear sub-pixel accumulators on collision
            ball->sub_x = 0;
            ball->sub_y = 0;
            
            // Stop if bounce is too weak
            if (ball->vy > -FIXED_EIGHTH && ball->vy < FIXED_EIGHTH) {
                ball->vy = 0;
                ball->vx = 0;
            }
        }
    }
}

void handle_ball_pin_collision(Ball* ball, Pin* pin)
{
    // 1. Calculate Ball Center (8-bit integer math)
    // Ball's center is ball->x + 4, ball->y + 4
    uint8_t ball_center_x = ball->x + TILE_HALF_WIDTH;
    uint8_t ball_center_y = ball->y + TILE_HALF_WIDTH;

    if (ball_center_x >= pin->x && 
        ball_center_x < (pin->x + SPRITE_SIZE) && 
        ball_center_y >= pin->y && 
        ball_center_y < (pin->y + SPRITE_SIZE)) {
        
        // calc offset
        int8_t pin_center_x = pin->x + TILE_HALF_WIDTH; 
        int8_t offset_x_int = ball_center_x - pin_center_x; 

        uint8_t magnitude_y_int = TILE_HALF_WIDTH - abs(offset_x_int);
        
        fixed_n force_x = PIN_BOUNCE_FORCE_H * offset_x_int; 
        fixed_n force_y = PIN_BOUNCE_FORCE_V * magnitude_y_int;

        // Apply negative sign to force_y to reflect the ball upwards
        apply_impulse(ball, force_x, -force_y);
    }

}