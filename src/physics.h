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
#include "customTypes.h"

// --- Physics constants (8.8 fixed-point) ---
#define GRAVITY     FIXED_TEENTH  
#define MAX_SPEED   TO_FIXED(10)
#define DAMPING     4       // bounce damping

// ##### GRAVITY #####

void apply_gravity(Ball *ball);
void apply_gravity_multi(Ball *balls, uint8_t count);

// ##### FORCE #####

void apply_impulse(Ball *ball, int8_t impulse_magnitude);

// ##### COLLISSION #####

void check_ball_wall(Ball *ball, Wall *w);
void check_ball_wall_multi(Ball *balls, Wall *w, uint8_t count);


#endif // PHYSICS_H
