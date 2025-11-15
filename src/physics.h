// physics.h

#ifndef PHYSICS_H
#define PHYSICS_H

#include <gb/gb.h>
#include <stdint.h>
#include "customTypes.h"

// --- Physics constants (8.8 fixed-point) ---
#define GRAVITY     FIXED_TEENTH   // 0.125 pixels/frame
#define MAX_SPEED   TO_FIXED(2)    // 10 pixels/frame max
#define DAMPING     4       // bounce damping

// --- Data types ---
typedef struct {
    fixed_n x, y;   // position in 8.8 fixed-point
    fixed_n  vx, vy; // velocity in 8.8 fixed-point
    GameSprite *game_sprite;
} Ball;

typedef struct {
    uint8_t x, y;       // top-left in pixels
    uint8_t width, height;
    GameSprite *game_sprite;
} Wall;

// --- Function prototypes ---
void apply_gravity(Ball *ball);
void apply_gravity_multi(Ball *balls, uint8_t count);

void check_ball_wall(Ball *ball, Wall *w);
void check_ball_wall_multi(Ball *balls, Wall *w, uint8_t count);

void apply_impulse(Ball *ball, int16_t impulse_magnitude);

#endif // PHYSICS_H
