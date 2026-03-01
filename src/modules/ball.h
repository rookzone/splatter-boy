// ball.h

#ifndef BALL_H
#define BALL_H

#include "../types.h"

// Module: Ball (gameplay example)
// Dependencies: graphics, physics, game_object (core)

void launch_ball_random(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t base_power_x, fixed_t base_power_y);

GameObject* spawn_ball(uint8_t x, uint8_t y);

void update_ball(GameObject* obj);

void ball_update_all(void);

void reset_all_balls(void);

void launch_ball(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t launch_power_x, fixed_t launch_power_y);

GameObject* find_lowest_ball(void);

// === MODULE WRAPPER ===
void module_ball_init(void);
void module_ball_update(void);
void module_ball_shutdown(void);

#endif // BALL_H
