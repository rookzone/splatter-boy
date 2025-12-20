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

#include "types.h"

// === FORCE ===

// Apply instant impulse force to ball
void apply_impulse(GameObject* obj, fixed_t impulse_magnitude_x, fixed_t impulse_magnitude_y);

// === BALL PHYSICS ===

void update_ball_position(GameObject* obj);
void apply_impulse(GameObject* obj, fixed_t impulse_x, fixed_t impulse_y);
void handle_ball_pin_collision(GameObject* obj);

// === LOOKUP TABLES ===

// Random horizontal velocities for ball resets
extern const fixed_t RANDOM_HORIZONTAL_VX[20];

#endif // PHYSICS_H_