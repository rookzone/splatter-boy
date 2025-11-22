#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"
#include "customtypes.h"

#ifndef BALL_H
#define BALL_H

// Reset an array of balls to starting positions.
void reset_balls(Ball* b, uint8_t count);

void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y);

void reset_ball(Ball* ball);

#endif // BALL_H
