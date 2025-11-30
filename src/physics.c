// Physics.c

#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "graphics.h"


void update_ball_position(Ball *ball)
{
    ball->vy += GRAVITY;

    if (ball->vy > MAX_SPEED)
        ball->vy = MAX_SPEED;
    
    ball->sub_x += ball->vx;
    ball->sub_y += ball->vy;

    // ** Optimization: Use direct calculation and assignment **
    ball->x += (int8_t)(ball->sub_x >> FIXED_SHIFT);
    ball->y += (int8_t)(ball->sub_y >> FIXED_SHIFT);
    
    ball->sub_x &= 0xFF; // Keep only the fractional part (8 bits)
    ball->sub_y &= 0xFF; // Keep only the fractional part (8 bits)

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
    // Calculate centers
    uint8_t ball_center_x = ball->x + TILE_HALF_WIDTH;
    uint8_t pin_center_x = pin->x + TILE_HALF_WIDTH;

    // Check horizontal distance
    int8_t distance_x = ball_center_x - pin_center_x;

    // If we are outide pin horizontal bounds
    // But still within collision tile (handle_ball_pin_collision is only called if ball is on pin tile)
    // then nudge ball and exit
    if (distance_x > PIN_HALF_WIDTH || distance_x < -PIN_HALF_WIDTH) {
    
        ball->vy = 0; 
        
        if (distance_x > 0) ball->vx = FIXED_QUARTER; 
        else ball->vx = -FIXED_QUARTER;
        
        return; 
    }

    // === collision ===

    // settle position on top of pin
    ball->y = pin->y - TILE_HALF_WIDTH;
    ball->sub_y = 0; 

    if (ball->vy > FIXED_EIGHTH) { // BOUNCE if vy is significant (fixed_eighth = 0.125)

        ball->vy = -(ball->vy >> 1); // 50% energy retention
        
        // apply horizontal impulse based on distance from center
        fixed_n impulse_x = FIXED_MUL(distance_x, HORIZONTAL_PIN_FORCE);
        ball->vx += impulse_x;

        
    } else { // ROLL if vy is small

        // stop vertical movement
        ball->vy = 0;
        
        // apply rolling force based on distance from center
        ball->vx += FIXED_MUL(distance_x, ROLL_FORCE); 

        // Clamp horizontal speed to prevent excessive rolling
        if (ball->vx > MAX_ROLL_SPEED) ball->vx = MAX_ROLL_SPEED;
        else if (ball->vx < -MAX_ROLL_SPEED) ball->vx = -MAX_ROLL_SPEED;
        
    }
}


void handle_ball_45_degree_wall_collision(Ball *ball, Wall *w)
{
    // Calculate visual point of collision for the "surface"

    // Settle the velocity and sub pixel accumulator

    // Determine if bounce or roll

    // If roll, skip low-energy micro-bounce and apply force into rolling motion

    // If bounce, apply basic axes swap

}

// Lookup tables

const fixed_n RANDOM_HORIZONTAL_VX[20] = {
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50    // -0.5
};


// end 