// Physics.c

#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "graphics.h"


void update_ball_position(Ball *ball)
{

    apply_gravity(ball);
    
    ball->sub_x += ball->vx;
    ball->sub_y += ball->vy;

    // ** Optimization: Use direct calculation and assignment **
    ball->x += (int8_t)(ball->sub_x >> FIXED_SHIFT);
    ball->y += (int8_t)(ball->sub_y >> FIXED_SHIFT);
    
    ball->sub_x &= 0xFF; // Keep only the fractional part (8 bits)
    ball->sub_y &= 0xFF; // Keep only the fractional part (8 bits)


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
            ball->vx = ball->vx - (ball->vx >> 2);
            
            // Stop if bounce is too weak (> 0.25 vx/y)
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

    // settle position
    ball->y = pin->y - TILE_HALF_WIDTH;
    ball->sub_y = 0;

    // invert and reduce vertical speed by 50%
    ball->vy = -(ball->vy >> 1);
    
    // Invert horizontal speed
    ball->vx = -(ball->vx);
    ball->vx -= FIXED_EIGHTH; // bleed off some speed after bouncing

    // give the ball a little nudge if it becomes settled on top the pin
    // TO TRY: Play around with the freshold or roll "strength" here.
    if (ball->vy > -FIXED_TENBAG) { 

        if (distance_x > 0) { // Hit right of center
            ball->vx += FIXED_TEENTH; 
        } else if (distance_x < 0) { // Hit left of center
            ball->vx += -FIXED_TEENTH; 
        }
    }
}