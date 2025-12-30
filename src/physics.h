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

// INLINED FOR SPEED
static inline void update_ball_position(GameObject* ball)
{
    PhysicsComponent* phys = &ball->physics;

    phys->vy += GRAVITY;

    if (phys->vy > MAX_SPEED)
        phys->vy = MAX_SPEED;

    phys->position_accumulator_x += phys->vx;
    phys->position_accumulator_y += phys->vy;

    // Apply the int component to screen position (left / high byte)
    ball->transform.x += ((int8_t*)&phys->position_accumulator_x)[1];
    ball->transform.y += ((int8_t*)&phys->position_accumulator_y)[1];

    // Zero out the whole number, keep the sub pixel decimal section for next loop
    ((int8_t*)&phys->position_accumulator_x)[1] = 0;
    ((int8_t*)&phys->position_accumulator_y)[1] = 0;
}

void check_ball_pin_collision(GameObject* obj);

// === LOOKUP TABLES ===

// Random horizontal velocities for ball resets
extern const fixed_t RANDOM_HORIZONTAL_VX[30];

#endif // PHYSICS_H_