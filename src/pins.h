#ifndef PINS_H
#define PINS_H

#include <gb/gb.h>
#include "physics.h"

typedef struct {
    uint8_t x, y;
    GameSprite* game_sprite;
} Pin;


Pin* init_pins(uint8_t* pin_positions[][], uint8_t count);

#endif
