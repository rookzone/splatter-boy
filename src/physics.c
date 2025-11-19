#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "graphics.h"

void apply_gravity(Ball *ball) 
{
    ball->vy += GRAVITY;
    
    if (ball->vy > MAX_SPEED) {
        ball->vy = MAX_SPEED;
    }
    
    ball->sub_x += ball->vx;
    ball->sub_y += ball->vy;
    
    int8_t dx = FROM_FIXED(ball->sub_x);  // >> 8
    int8_t dy = FROM_FIXED(ball->sub_y);  // >> 8
    
    ball->x += dx;
    ball->y += dy;
    
    ball->sub_x -= TO_FIXED(dx);  // - (dx << 8)
    ball->sub_y -= TO_FIXED(dy);  // - (dy << 8)
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

        gotogxy(12,0);
        printf("collision!");
        gotogxy(0,0);

        
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
