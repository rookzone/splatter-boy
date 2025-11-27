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

// New constant for 80% elasticity (205 / 256 ~= 0.80)
#define PIN_ELASTICITY_80 205 
// Roll force is now FIXED_EIGHTH (0.125) which is double the previous FIXED_TEENTH (0.0625)
#define ROLL_FORCE FIXED_EIGHTH  
// Max rolling speed is 0.75 pixels/frame
#define ROLL_SPEED_MAX (FIXED_HALF + FIXED_QUARTER)

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

    // Check if this is a bounce (falling onto pin) or resting (settled on top)

    if (ball->vy > FIXED_EIGHTH) { // BOUNCE

        // invert vy and add speed loss component
        ball->vy = -(ball->vy >> 1);
        
        // Invert vx and add speed loss component
        ball->vx = -(ball->vx >> 1);
        

    } else {
// Cancel vertical velocity to stick to pin surface
        ball->vy = 0;
        
        // Apply gentle rolling force proportional to offset from center.
        // ROLL_FORCE is now higher (FIXED_EIGHTH) for a snappier start.
        ball->vx += FIXED_MUL(distance_x, ROLL_FORCE);
        
        // Limit the horizontal speed to the new, higher max (0.75 pixels/frame)
        if (ball->vx > ROLL_SPEED_MAX) ball->vx = ROLL_SPEED_MAX;
        else if (ball->vx < -ROLL_SPEED_MAX) ball->vx = -ROLL_SPEED_MAX;
    }
}

/*
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
    ball->vx = -(ball->vx >> 1);
   // ball->vx -= FIXED_TEENTH; // bleed off some speed after bouncing

    ball->vx += (distance_x >> 2);
}
*/