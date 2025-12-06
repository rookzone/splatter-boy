// ball.h


#ifndef BALL_H_
#define BALL_H_

#include "custom_types.h"
#include "game_object.h"
#include "game_data.h"

#define LAUNCH_FORCE_X TO_FIXED(2)
#define LAUNCH_FORCE_Y TO_FIXED(2)

GameObject* spawn_ball(uint8_t x, uint8_t y);

void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y);

// Update
void update_ball(GameObject* obj);

// Reset an array of balls to starting positions.
void reset_all_balls(void);

// Reset balls to a position (for testing)
void reset_ball(Ball* ball);

// Return the ball with the highest y index (lowest on screen)
Ball *find_lowest_ball(Ball* balls, uint8_t count);

// Launch the ball from screen position from_x,y
// Default is 45 degrees
void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y);


#endif // BALL_H_