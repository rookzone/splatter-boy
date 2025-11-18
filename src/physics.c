#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "graphics.h"

void apply_gravity(Ball *ball) 
{
        ball->vy += GRAVITY;

        ball->y += FROM_FIXED(ball->vy);
        ball->x += FROM_FIXED(ball->vx);

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

        // Convert position to integer pixel Y
        uint8_t sprite_y = ball->y + 8;
       // printf("%u", w->y);

        if (sprite_y >= w->y + 8) {

            // 2. Calculate new (damped) bounce velocity
            int16_t bounce_vy = FROM_FIXED(-ball->vy + FIXED_EIGHTH); // SAME AS /4;

            ball->y = 0;

            // 1. Position correction
            ball->y = w->y;
            ball->vx = 0;
            ball->vy = 0;

            // BOUNCE NOT WORKING. RECALC PHYSX
            //If the bounce velocity is tiny (e.g., less than FIXED_EIGHTH or 0.125), stop it entirely.
            if (bounce_vy < 1 && bounce_vy > -1) {
                ball->vy = 0; // Ball is fully settled (no more vertical movement)
                ball->vx = 0; // Stop any sliding (no more horizontal movement)
            } else {
                // It's still bouncing, apply the calculated velocities
                ball->vy = bounce_vy / 4;
                //ball->vx = ball->vx >> 1;
                //ball->vx /= 2;
            }
                                
            // Add a random impulse
            //apply_impulse(ball,-20,0);

        }
    }
}


/*
void check_ball_pin_collision_point_in_box(Ball* ball, Pin* pin)
{
    // 1. Calculate Ball Center (8-bit integer math)
    // Ball's center is ball->x + 4, ball->y + 4
    uint8_t ball_center_x = ball->x + TILE_HALF_WIDTH;
    uint8_t ball_center_y = ball->y + TILE_HALF_WIDTH;
    
    // 2. Optimized Point-in-Box Check (Pin's bounding box)
    // The pin's box is from pin->x to pin->x + 8
    
    if (ball_center_x >= pin->x && 
        ball_center_x < (pin->x + SPRITE_SIZE) && 
        ball_center_y >= pin->y && 
        ball_center_y < (pin->y + SPRITE_SIZE)) 
    {
        // 3. Collision detected! Now switch to fixed_n for precise impulse.
        
        // Pin center x (integer) is pin->x + 4
        int8_t pin_center_x = pin->x + TILE_HALF_WIDTH; 
        
        // offset_x_int is the signed integer distance from pin center to ball center
        int8_t offset_x_int = ball_center_x - pin_center_x; 

        // Convert the integer offset to fixed_n *once* for the physics calculation.
        fixed_n offset_x_fixed = TO_FIXED(offset_x_int); 
        
        // Calculate force_x (Horizontal/Reflection component):
        // Formula: PIN_BOUNCE_FORCE_H * offset_x_int
        fixed_n force_x = PIN_BOUNCE_FORCE_H * offset_x_int; 
        
        // Calculate magnitude_y_int (Vertical/Forward component):
        // Magnitude is proportional to (4 - |offset_x_int|).
        uint8_t magnitude_y_int = TILE_HALF_WIDTH - abs(offset_x_int);

        // Calculate force_y: PIN_BOUNCE_FORCE_V * magnitude_y_int
        fixed_n force_y = PIN_BOUNCE_FORCE_V * magnitude_y_int;

        // Apply negative sign to force_y to reflect the ball upwards
        apply_impulse(ball, force_x, -force_y);
    } 
}
    */
