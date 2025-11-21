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

// Ball: 8x8, center at +4
// Pin: 4x4 centered in 8x8 tile, so center at +4 (same)
// Combined radii: 4 (ball) + 2 (pin) = 6
#define PIN_COLLISION_DIST 6

void handle_ball_pin_collision(Ball* ball, Pin* pin)
{
    // Both centers are at +4 from top-left, so offset cancels out
    int8_t dx = ball->x - pin->x;
    int8_t dy = ball->y - pin->y;
    
    // Quick rejection (using collision dist of 6)
    if ((uint8_t)(dx + 5) >= 11) return;
    if ((uint8_t)(dy + 5) >= 11) return;
    
    int8_t abs_dx = dx < 0 ? -dx : dx;
    int8_t abs_dy = dy < 0 ? -dy : dy;
    
    if (abs_dx >= PIN_COLLISION_DIST || abs_dy >= PIN_COLLISION_DIST) return;
    
    // Reflect velocity component that points into the pin
    int8_t moving_toward_x = (dx > 0 && ball->vx < 0) || (dx < 0 && ball->vx > 0);
    int8_t moving_toward_y = (dy > 0 && ball->vy < 0) || (dy < 0 && ball->vy > 0);
    
    if (abs_dx >= abs_dy) {
        if (moving_toward_x) {
            ball->vx = -ball->vx;
        }
        int8_t transfer = ball->vx >> 3;
        ball->vy += (dy > 0) ? transfer : -transfer;
        ball->vx -= (dy > 0) ? transfer : -transfer;
    } else {
        if (moving_toward_y) {
            ball->vy = -ball->vy;
        }
        int8_t transfer = ball->vy >> 3;
        ball->vx += (dx > 0) ? transfer : -transfer;
        ball->vy -= (dx > 0) ? transfer : -transfer;
    }
    
    // Separation push
    uint8_t overlap = PIN_COLLISION_DIST - (abs_dx > abs_dy ? abs_dx : abs_dy);
    if (dx > 0) ball->x += (overlap >> 1) + 1;
    else ball->x -= (overlap >> 1) + 1;
    if (dy > 0) ball->y += (overlap >> 1) + 1;
    else ball->y -= (overlap >> 1) + 1;
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
    int16_t distance_x = abs(ball_center_x - pin_center_x);
    int16_t distance_y = abs(ball_center_y - pin_center_y);

    // if ball is 16 or more units away then skip collision check
    if(distance_x >= SPRITE_SIZE || distance_y >= SPRITE_SIZE){
        goto finish;
    }
        
    
    if (ball_center_x >= pin->x && 
        ball_center_x < (pin->x + SPRITE_SIZE) && 
        ball_center_y >= pin->y && 
        ball_center_y < (pin->y + SPRITE_SIZE)) {
        
        // calc offset
        int8_t offset_x_int = distance_x;

        uint8_t magnitude_y_int = TILE_HALF_WIDTH - offset_x_int;
        
        fixed_n force_x = PIN_BOUNCE_FORCE_H * offset_x_int; 
        fixed_n force_y = PIN_BOUNCE_FORCE_V * magnitude_y_int;

        // Apply negative sign to force_y to reflect the ball upwards
        apply_impulse(ball, force_x, -force_y);
        
        }
    

    finish:

}
*/
/*
// Collision distance = ball radius (4) + pin radius (2) = 6
#define COLLISION_DIST 6
#define COLLISION_DIST_MINUS_1 5

void handle_ball_pin_collision(Ball* ball, Pin* pin)
{
    // Delta between centers
    // Ball center: x + 4, Pin center: x + 2 (smaller sprite)
    // So dx = (ball->x + 4) - (pin->x + 2) = ball->x - pin->x + 2
    int8_t dx = ball->x - pin->x + 2;
    int8_t dy = ball->y - pin->y + 2;
    
    // Quick rejection
    if ((uint8_t)(dx + COLLISION_DIST_MINUS_1) >= (COLLISION_DIST * 2 - 1)) return;
    if ((uint8_t)(dy + COLLISION_DIST_MINUS_1) >= (COLLISION_DIST * 2 - 1)) return;
    
    int8_t abs_dx = dx < 0 ? -dx : dx;
    int8_t abs_dy = dy < 0 ? -dy : dy;
    
    if (abs_dx >= COLLISION_DIST || abs_dy >= COLLISION_DIST) return;
    
    // --- COLLISION DETECTED ---
    
    if (abs_dx >= abs_dy) {
        // Horizontal collision
        if ((dx > 0 && ball->vx < 0) || (dx < 0 && ball->vx > 0)) {
            ball->vx = -ball->vx;
        }
        ball->vy += dy << 4;  // Influence in fixed-point
        ball->x = (dx >= 0) ? pin->x - 2 + COLLISION_DIST : pin->x + 2 - COLLISION_DIST;
    } else {
        // Vertical collision
        if ((dy > 0 && ball->vy < 0) || (dy < 0 && ball->vy > 0)) {
            ball->vy = -ball->vy;
        }
        ball->vx += dx << 4;  // Influence in fixed-point
        ball->y = (dy >= 0) ? pin->y - 2 + COLLISION_DIST : pin->y + 2 - COLLISION_DIST;
    }
}

*/