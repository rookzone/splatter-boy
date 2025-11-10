#ifndef PINS_H
#define PINS_H

#include <gb/gb.h>
#include "physics.h"

typedef struct {
    uint8_t x, y;
    uint8_t sprite_id;
} Pin;

void init_pins(const uint8_t pin_positions[][2], uint8_t count);
void draw_pins(void);
void check_pin_collisions(Ball *ball, Pin *pins, uint8_t count);

#endif
