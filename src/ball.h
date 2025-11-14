#ifndef BALL_H
#define BALL_H

#include <gb/gb.h>
#include "physics.h"

enum { BALL_SPRITE = 0, WALL_SPRITE = 1 };
enum { TILE_BALL = 0, TILE_WALL = 1 };

void init_balls(Ball* b, uint8_t count);
void reset_balls(Ball* b, uint8_t count);



#endif // BALL_H
