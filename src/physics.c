#include "physics.h"
#include "graphics.h"

void apply_gravity(Ball *ball) 
{
        ball->vy += GRAVITY;

        ball->y += ball->vy;
        ball->x += ball->vx;

        if (ball->vy > MAX_SPEED) {
            ball->vy = MAX_SPEED;
        }
}

void apply_impulse(Ball *ball, int16_t impulse_magnitude)
{

    ball->vx += impulse_magnitude;

}

void check_ball_wall(Ball *ball, Wall *w) 
{
    if (ball->vy > 0) { 

        // Convert position to integer pixel Y
        uint16_t sprite_y = FROM_FIXED(ball->y);

        if (sprite_y + 8 >= w->y) {

            // 1. Position correction
            ball->y = TO_FIXED(w->y - 8);
            ball->vx = 0;

            // 2. Calculate new (damped) bounce velocity
            fixed_n bounce_vy = -ball->vy >> 2; // SAME AS /4;

            // If the bounce velocity is tiny (e.g., less than FIXED_EIGHTH or 0.125), stop it entirely.
            if (bounce_vy < FIXED_EIGHTH && bounce_vy > -FIXED_EIGHTH) {
                ball->vy = 0; // Ball is fully settled (no more vertical movement)
                ball->vx = 0; // Stop any sliding (no more horizontal movement)
            } else {
                // It's still bouncing, apply the calculated velocities
                ball->vy = bounce_vy;
                //ball->vx = ball->vx >> 1;
                //ball->vx /= 2;
                                
            // Add a random impulse
            apply_impulse(ball,-20);
            }
        }

    }
}

void check_ball_pin_collision(Ball* ball, Pin* pin)
{

    
}
