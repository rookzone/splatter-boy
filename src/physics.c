#include <gb/gb.h>
#include "physics.h"

void apply_gravity(Ball *ball) 
{
    if (ball->vy < MAX_SPEED) {
        ball->vy += GRAVITY;
    }
    ball->y += ball->vy;
    ball->x += ball->vx;
}

void check_ball_wall(Ball *ball, Wall *w) 
{
    uint16_t sprite_y = ball->y >> 8;
    if (sprite_y + 8 >= w->y) {
        ball->y = (w->y - 8) << 8;
        ball->vy = -ball->vy >> 2; // invert velocity and then divide by 4 (shift 2 bits left)
    }
}

void apply_impulse(Ball *ball, uint8_t impulse) 
{
    ball->vy -= impulse;
}

void apply_gravity_multi(Ball *balls, uint8_t count) 
{
    for (uint8_t i = 0; i < count; i++) {
        if (balls[i].vy < MAX_SPEED) {
            balls[i].vy += GRAVITY;
        }
        balls[i].y += balls[i].vy;
        balls[i].x += balls[i].vx;

        move_sprite(2 + i, FROM_FIXED(balls[i].x), FROM_FIXED(balls[i].y));
    }
}

void check_ball_wall_multi(Ball *balls, Wall *w, uint8_t count) 
{
    for (uint8_t i = 0; i < count; i++) {
        uint16_t sprite_y = balls[i].y >> 8;
        if (sprite_y + 8 >= w->y) {
            balls[i].y = (w->y - 8) << 8;
            balls[i].vy = -balls[i].vy / DAMPING;
            balls[i].vx /= 2;
        }
    }
}
