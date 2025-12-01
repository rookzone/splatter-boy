// ball.h


#ifndef BALL_H_
#define BALL_H_

#include "custom_types.h"

#define LAUNCH_FORCE_X TO_FIXED(2)
#define LAUNCH_FORCE_Y TO_FIXED(2)

// Reset an array of balls to starting positions.
void reset_balls(Ball* b, uint8_t count);

// Pass in ball object alongside graphics data and screen position and it will be set up for handling
void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y);

// Reset balls to a position (for testing)
void reset_ball(Ball* ball);

// Return the ball with the highest y index (lowest on screen)
Ball *find_lowest_ball(Ball* balls, uint8_t count);

// Launch the ball from screen position from_x,y
// Default is 45 degrees
void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y);

#endif // BALL_H_
