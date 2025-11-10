// physics.h

#ifndef PHYSICS_H
#define PHYSICS_H

#include <gb/gb.h>
#include <stdint.h>

// --- Macros ---
#define FIXED(x) ((int16_t)((x) * 256))

// --- Physics constants (8.8 fixed-point) ---
#define GRAVITY     32      // 0.125 pixels/frame
#define MAX_SPEED   2560    // 10 pixels/frame max
#define DAMPING     4       // bounce damping

// --- Data types ---
typedef struct {
    uint16_t x, y;   // position in 8.8 fixed-point
    int16_t  vx, vy; // velocity in 8.8 fixed-point
} Ball;

typedef struct {
    uint8_t x, y;       // top-left in pixels
    uint8_t width, height;
} Wall;

// --- Function prototypes ---
void apply_gravity(Ball *b);
void apply_gravity_multi(Ball *b, uint8_t count);

void check_ball_wall(Ball *b, Wall *w);
void check_ball_wall_multi(Ball *b, Wall *w, uint8_t count);

void apply_impulse(Ball *b, uint8_t impulse);

// Optionally uncomment if you bring init_balls() back later
// void init_balls(Ball *b, uint8_t count);

#endif // PHYSICS_H
