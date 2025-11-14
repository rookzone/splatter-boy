#ifndef BALL_H
#define BALL_H

#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"

void init_balls(Ball* b, GameSprite* gfx_data, uint8_t count);
void reset_balls(Ball* b, uint8_t count);



#endif // BALL_H
