// physics.h

#ifndef PHYSICS_H
#define PHYSICS_H

#include <gb/gb.h>
#include <stdint.h>
#include "customtypes.h"

// Physics constants (8.8 fixed-point)
#define GRAVITY     FIXED_TEENTH  // 256/16
#define MAX_SPEED   TO_FIXED(2) // Max speed of balls in fixed-point number space
#define PIN_HALF_WIDTH 2 
#define ROLL_FORCE FIXED_EIGHTH

// Applies forces and updates ball position in fixed-point number space (does NOT render)
void update_ball_position(Ball *ball);

// === FORCE ===

// Apply instant impulse force to ball
void apply_impulse(Ball *ball, fixed_n impulse_magnitude_x, fixed_n impulse_magnitude_y);

// === COLLISSION ===

// Handle collision between a Ball and Wall.
void check_ball_wall(Ball *ball, Wall *w);

// Handle collision between a Ball and Pin.
void handle_ball_pin_collision(Ball *ball, Pin *pin);

// === LOOKUP TABLES ===

// Random horizontal velocities for ball resets
extern const fixed_n RANDOM_HORIZONTAL_VX[20];

#endif // PHYSICS_H
