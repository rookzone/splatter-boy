#include <stdlib.h>
#include <stdio.h>
#include "physics.h"
#include "graphics.h"

void update_ball_position(Ball *ball)
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


void check_ball_wall(Ball *ball, Wall *wall) 
{
    if (ball->vy > 0) { 
        uint8_t ball_bottom = ball->y + SPRITE_SIZE;

        if (ball_bottom >= wall->y) {

            // Correct position
            ball->y = wall->y - SPRITE_SIZE;
            
            // Bounce (50% energy retention)
            ball->vy = -(ball->vy >> 1);
            
            // Friction (75% horizontal speed)
            ball->vx = (ball->vx * 3) >> 2;

            
            // Stop if bounce is too weak
            if (ball->vy > -FIXED_QUARTER) {
                ball->vy = 0;
                ball->vx = 0;
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
    int16_t distance_x = abs(ball_center_x - pin_center_x);
    int16_t distance_y = abs(ball_center_y - pin_center_y);

    // if ball is 16 or more units away then skip collision check
    if(distance_x >= SPRITE_SIZE || distance_y >= SPRITE_SIZE){
        goto finish;
    }
        
    if (ball_center_x >= pin->x && 
        ball_center_x < (pin->x + SPRITE_SIZE) && 
        ball_center_y >= pin->y && 
        ball_center_y < (pin->y + SPRITE_SIZE)) || ball->is_colliding == false) {

            // invert velocity
            ball->vx = -ball->vx;
            ball->vy = -ball->vy;

            ball->is_colliding = true;
        
        // calc offset
        int8_t offset_x_int = ball_center_x - pin_center_x; 

        uint8_t magnitude_y_int = TILE_HALF_WIDTH - abs(offset_x_int);
        
        fixed_n force_x = PIN_BOUNCE_FORCE_H * offset_x_int; 
        fixed_n force_y = PIN_BOUNCE_FORCE_V * magnitude_y_int;

        // Apply negative sign to force_y to reflect the ball upwards
        apply_impulse(ball, force_x, -force_y);
    }

