// ball.h


#ifndef BALL_H_
#define BALL_H_

#include "custom_types.h"
#include "game_object.h"
#include "game_data.h"

#define NUM_BALLS 18

#define LAUNCH_FORCE_X TO_FIXED(2)
#define LAUNCH_FORCE_Y TO_FIXED(2)

GameObject* spawn_ball(uint8_t x, uint8_t y);

void update_ball(GameObject* obj);

void reset_all_balls(void);

void launch_ball(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y);

GameObject* find_lowest_ball(void);

#endif // BALL_H_