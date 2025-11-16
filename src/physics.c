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
                ball->vx = ball->vx >> 1;
                //ball->vx /= 2;
            }
        }

    }
}

void apply_impulse(Ball *ball, int8_t impulse_magnitude)
{

    ball->vx += impulse_magnitude;
}

void apply_gravity_multi(Ball *balls, uint8_t count) 
{
    for (uint8_t i = 0; i < count; i++) {

        balls[i].vy += GRAVITY;

        balls[i].y += balls[i].vy;
        balls[i].x += balls[i].vx;

        if (balls[i].vy > MAX_SPEED) {
        balls[i].vy = MAX_SPEED;
        }

    }
}

void check_ball_wall_multi(Ball *balls, Wall *w, uint8_t count) 
{
    for (uint8_t i = 0; i < count; i++) {
        
        // --- 1. Culling/Skip Check (The biggest saving) ---
        // Only run the collision math if the ball is moving downwards (vy > 0)
        if (balls[i].vy > 0) { 

            // Convert position to integer pixel Y
            uint16_t sprite_y = balls[i].y >> 8;
            

            if (sprite_y + 8 >= w->y) {
                
                // --- Collision Response ---
                
                // 1. Position correction
                balls[i].y = (w->y - 8) << 8;
                
                // 2. Calculate new (damped) bounce velocity
                fixed_n bounce_vy = -balls[i].vy / DAMPING;
                
                // --- 3. Settling Logic (Saves future CPU time) ---
                // If the bounce velocity is tiny (e.g., less than FIXED_EIGHTH or 0.125), stop it entirely.
                if (bounce_vy < FIXED_EIGHTH && bounce_vy > -FIXED_EIGHTH) {
                    balls[i].vy = 0; // Ball is fully settled (no more vertical movement)
                    balls[i].vx = 0; // Stop any sliding (no more horizontal movement)
                } else {
                    // It's still bouncing, apply the calculated velocities
                    balls[i].vy = bounce_vy;
                    balls[i].vx /= 2;
                }
            }
        }
    }
}
