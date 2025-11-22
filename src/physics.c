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

    // if ball is more than 8 units away, skip check
    if(distance_x >= SPRITE_SIZE || distance_y >= SPRITE_SIZE)
        return;


    // Ball is within collision bounds and colliding is false
    if ((ball_center_x >= pin->x && 
        ball_center_x < (pin->x + SPRITE_SIZE) && 
        ball_center_y >= pin->y && 
        ball_center_y < (pin->y + SPRITE_SIZE)) || ball->is_colliding == false) {

            // invert velocity
            ball->vx = -ball->vx;
            ball->vy = -ball->vy;

            ball->is_colliding = true;
        
    } else {
        ball->is_colliding = false; // If ball isn't in bounds but IS within range then colliding = false;
    }

}


