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
        uint8_t ball_bottom = ball->y + SPRITE_SIZE;

        if (ball_bottom >= w->y) {
            // Correct position
            ball->y = w->y - SPRITE_SIZE;
            ball->sub_y = 0;
            
            // Bounce (50% energy retention)
            ball->vy = -(ball->vy >> 1);
            
            // Friction (75% horizontal speed)
            ball->vx = (ball->vx * 3) >> 2;
            
            // Stop if bounce is too weak
            if (ball->vy > -FIXED_QUARTER) {
                ball->vy = 0;
                ball->vx = 0;
                ball->sub_x = 0;
            }
        }
    }
}

void handle_ball_pin_collision(Ball* ball, Pin* pin)
{
    // Grab center point of objects
    uint8_t ball_center_x = ball->x + TILE_HALF_WIDTH;
    uint8_t ball_center_y = ball->y + TILE_HALF_WIDTH;
    uint8_t pin_center_x = pin->x + TILE_HALF_WIDTH;
    uint8_t pin_center_y = pin->y + TILE_HALF_WIDTH;

    // Calculate distance of object
    int8_t distance_x = ball_center_x - pin_center_x;
    int8_t distance_y = ball_center_y - pin_center_y;

    // if ball is 8 or more units away then skip collision check
    if(abs(distance_x) >= SPRITE_SIZE || abs(distance_y) >= SPRITE_SIZE){
        return;
    }
        
    
    if (ball_center_x >= (pin->x + 2) && 
        ball_center_x < (pin->x + 6) && 
        ball_center_y >= (pin->y + 2) && 
        ball_center_y < (pin->y + 6)) {

        // settle position
        ball->y = pin->y - TILE_HALF_WIDTH;
        ball->sub_y = 0;

        // invert and reduce vertical speed by 50%
        ball->vy = -(ball->vy >> 1);
        
        // Reduce horizontal speed by 50%
        ball->vx = -(ball->vx);
    
    }
}