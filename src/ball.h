#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"
#include "customtypes.h"

#ifndef BALL_H
#define BALL_H

#define LAUNCH_FORCE_X TO_FIXED(2)
#define LAUNCH_FORCE_Y TO_FIXED(2)

// Reset an array of balls to starting positions.
void reset_balls(Ball* b, uint8_t count);

void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y);

void reset_ball(Ball* ball);

Ball* find_lowest_ball(Ball* balls, uint8_t count);

// Launch the ball from screen position from_x,y
// Default is 45 degrees
void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y);

#endif // BALL_H
