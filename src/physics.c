#include <gb/gb.h>
#include "physics.h"

void apply_gravity(Ball *b) {
    if (b->vy < MAX_SPEED) {
        b->vy += GRAVITY;
    }
    b->y += b->vy;
    b->x += b->vx;  // include horizontal motion if you want
}

void apply_gravity_multi(Ball *b, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        if (b[i].vy < MAX_SPEED) {
            b[i].vy += GRAVITY;
        }
        b[i].y += b[i].vy;
        b[i].x += b[i].vx;

        move_sprite(2 + i, b[i].x >> 8, b[i].y >> 8);
    }
}

void check_ball_wall(Ball *b, Wall *w) {
    uint16_t sprite_y = b->y >> 8;
    if (sprite_y + 8 >= w->y) {
        b->y = (w->y - 8) << 8;
        b->vy = -b->vy / DAMPING;
        b->vx /= 2;  // optional horizontal friction
    }
}

void check_ball_wall_multi(Ball *b, Wall *w, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        uint16_t sprite_y = b[i].y >> 8;
        if (sprite_y + 8 >= w->y) {
            b[i].y = (w->y - 8) << 8;
            b[i].vy = -b[i].vy / DAMPING;
            b[i].vx /= 2;
        }
    }
}

void apply_impulse(Ball *b, uint8_t impulse) {
    b->vy -= impulse;
}
