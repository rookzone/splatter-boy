// ball.h

#ifndef BALL_H_
#define BALL_H_

#include "_types.h"

void launch_ball_random(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t base_power_x, fixed_t base_power_y);

GameObject* spawn_ball(uint8_t x, uint8_t y);

void update_ball(GameObject* obj);

void reset_all_balls(void);

void launch_ball(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t launch_power_x, fixed_t launch_power_y);

GameObject* find_lowest_ball(void);

#endif // BALL_H_