/**
 * @file physics.h
 * @brief Contains physics functionality to the Game Objects.
 * * @details This contains functions for object movement and interactions.
 * The physics is not true to life but is instead optimised for speed.
 * As long as it looks good and it's cheap then that's fine.
 * We have constants that define certain forces like gravity and roll speed.
 * All physics interactions take place in "fixed-number space".
 * This means that the fixed number decimals are used for higher precision.
 * The update_position functions will convert the numbers back to integer and set x,y.
 * * @defgroup Physics
 */
#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "custom_types.h"

// Physics constants (8.8 fixed-point)
#define GRAVITY     FIXED_TEENTH  // 256/16
#define MAX_SPEED   TO_FIXED(2) // Max speed of balls in fixed-point number space
#define PIN_HALF_WIDTH 2 // half of pin pixel width (used for collisions)
#define ROLL_FORCE FIXED_QUARTER // Amount of force applied to ball to continue roll
#define MAX_ROLL_SPEED FIXED_HALF+FIXED_QUARTER // Max speed ball can go while rolling
#define HORIZONTAL_PIN_FORCE FIXED_QUARTER // For bounce force on x-axis

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

// Reflects ball from wall or sets it "rolling" along wall at 45 degree angle
void handle_ball_45_degree_wall_collision(Ball *ball, Wall *w);

// === LOOKUP TABLES ===

// Random horizontal velocities for ball resets
extern const fixed_n RANDOM_HORIZONTAL_VX[20];

#endif // PHYSICS_H_
