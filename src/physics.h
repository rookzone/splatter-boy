// physics.h

/*
#TODO

[ ] - Use this formula to create a collission and then deflect from pachinko_pin function, this adds an offset depending on which side of the pin the ball hits
  when hitting the center some randomness needs to occur...
// THIS IS THE ACTUAL COLLISION POINT FOR CALCULATIONS
        plot_point_fixed(ball->x - TO_FIXED(4), ball->y - TO_FIXED(8)); // plot collision point for debugging purposes
  
*/

#ifndef PHYSICS_H
#define PHYSICS_H

#include <gb/gb.h>
#include <stdint.h>
#include "customtypes.h"

// --- Physics constants (8.8 fixed-point) ---
#define GRAVITY     FIXED_TEENTH  
#define MAX_SPEED   TO_FIXED(4)
#define DAMPING     4       // bounce damping
#define PIN_BOUNCE_FORCE_H 16
#define PIN_BOUNCE_FORCE_V 64


void apply_gravity(Ball *ball);

// Applies forces and updates ball position in logical fixed number space (does NOT render)
void update_ball_position(Ball *ball);

// ##### FORCE #####

// Apply instant impulse force to ball
void apply_impulse(Ball *ball, fixed_n impulse_magnitude_x, fixed_n impulse_magnitude_y);

// ##### COLLISSION #####

// Handle collision between a Ball and Wall.
void check_ball_wall(Ball *ball, Wall *w);

// Handle collision between a Ball and Pin.
void handle_ball_pin_collision(Ball *ball, Pin *pin);

#endif // PHYSICS_H
