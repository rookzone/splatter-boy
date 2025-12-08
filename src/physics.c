// physics.c

#include <gb/gb.h>
#include "physics.h"
#include "graphics.h"

void update_ball_position(Ball *ball)
{
    ball->vy += GRAVITY;

    if (ball->vy > MAX_SPEED)
        ball->vy = MAX_SPEED;
    
    // Add our velocity to the fixed number fraction left over from last frame
    ball->fractional_x += ball->vx;
    ball->fractional_y += ball->vy;

    // Convert fixed value back to signed integer and apply to screen position
    ball->x += (int8_t)(ball->fractional_x >> FIXED_SHIFT);
    ball->y += (int8_t)(ball->fractional_y >> FIXED_SHIFT);
    
    // Zero off the left-hand byte to leave us with the decimal
    ball->fractional_x &= 0xFF;
    ball->fractional_y &= 0xFF;

}

void apply_impulse(Ball *ball, fixed_n impulse_magnitude_x, fixed_n impulse_magnitude_y)
{
    ball->vx += impulse_magnitude_x;
    ball->vy += impulse_magnitude_y;
}

void check_ball_wall(Ball *ball, Wall *w) 
{
    if (ball->vy > 0) { 
        uint8_t ball_bottom = ball->y + SPRITE_SIZE;

        if (ball_bottom >= w->y) {

            // Correct position
            ball->y = w->y - SPRITE_SIZE;
            ball->fractional_y = 0;
            
            // Bounce (50% energy retention)
            ball->vy = -(ball->vy >> 1);
            ball->vx = ball->vx - (ball->vx >> 2);
            
            // Stop if bounce is too weak (> 0.25 vx/y)
            if (ball->vy > -FIXED_QUARTER) {
                ball->vy = 0;
                ball->vx = 0;
                ball->fractional_x = 0;
            }
        }
    }
}

/**
 * @brief Checks a Ball for any collisions with pins, if so applies a bounce
 *  @todo Order of checks could be optimised
 * Change the pins so they are at the top of the tile...
 * My god this likely needs optimising the hell out of.
 * Although filters work well, unlikely to be lots of balls in actual collision simultaeneousy
 * @param ball The Ball object reference to check 
 */
void handle_ball_pin_collision(Ball* ball)
{
    
    // FILTER: Only handle collision if ball is moving downward
    if (ball->vy <= 0)
        return;

    // Get ball's bottom-middle point
    uint8_t ball_bottom_x = ball->x + TILE_HALF_WIDTH;
    uint8_t ball_bottom_y = ball->y + TILE_WIDTH;

    // Find which 8x8 tile the ball's bottom is in
    uint8_t col = PIXEL_TO_GRID(ball_bottom_x);
    uint8_t row = PIXEL_TO_GRID(ball_bottom_y);

    // FILTER: Outside screen, do not collide
    if (col >= BACKGROUND_WIDTH_TILES || row >= BACKGROUND_HEIGHT_TILES)
        return;

    // Grab the tiles index value
    uint16_t tilemap_index = GET_TILE_INDEX(col, row);

    // FILTER: Check the tile in the map at that index, if it's a pin tile we need to collide
    if (get_game_background_tilemap()[tilemap_index] != PIN_TILE_ID)
        return;
            
    // Get coords of pin tile
    uint8_t pin_tile_x = GRID_TO_PIXEL(col);
    uint8_t pin_tile_y = GRID_TO_PIXEL(row);

    // Calculate horizontal distances to determine if ball will hit pin
    uint8_t pin_center_x = pin_tile_x + TILE_HALF_WIDTH;
    int8_t distance_x = ball_bottom_x - pin_center_x;
    
    if (distance_x < -(PIN_HALF_WIDTH) || distance_x > (PIN_HALF_WIDTH))
        return;

    // Calculate vertical distance so collision only happens when top of pin is hit
    uint8_t pin_visual_top = pin_tile_y + PIN_HALF_WIDTH;
    int8_t vertical_dist = ball_bottom_y - pin_visual_top;
    
    // Distance for collision is 0 - 4, so anything above 4 or less than 0 SKIP
    if (vertical_dist < 0 || vertical_dist > TILE_HALF_WIDTH)
        return;

    // Zero fractional velocity
    ball->fractional_y = 0;

    if (ball->vy > FIXED_QUARTER) { 
        // === BOUNCE ===
        
        // Vertical bounce (50% energy)
        ball->vy = -(ball->vy >> 1);
        
        // If ball moving right and hits left side (distance_x < 0), bounce left
        // If ball moving left and hits right side (distance_x > 0), bounce right
        if ((ball->vx > 0 && distance_x < 0) || (ball->vx < 0 && distance_x > 0)) {
            // Hit the "wrong" side - reverse direction with damping
            ball->vx = -(ball->vx >> 1);
        } else {
            // Hit the "correct" side - keep direction but add deflection force
            ball->vx = (ball->vx >> 1);
            
            // Add a nudge based on offset from center (scaled down)
            if (distance_x > 0) {
                ball->vx += FIXED_EIGHTH;  // nudge right
            } else if (distance_x < 0) {
                ball->vx -= FIXED_EIGHTH;  // nudge left
            } else {
                // Dead center hit - give it a small nudge to break symmetry
                ball->vx += (ball->y & 1) ? FIXED_EIGHTH : -FIXED_EIGHTH;
            }
        }
        
    } else { 
        // === ROLL ===
        
        ball->vy = 0;
        
        // Shift distance_x to scale it down
        ball->vx += (distance_x >> 2);
        
        // Clamp horizontal speed
        if (ball->vx > MAX_ROLL_SPEED) ball->vx = MAX_ROLL_SPEED;
        else if (ball->vx < -MAX_ROLL_SPEED) ball->vx = -MAX_ROLL_SPEED;
    }
}

void handle_ball_45_degree_wall_collision(Ball *ball, Wall *w)
{
    // Calculate visual point of collision for the "surface"

    // Settle the velocity and sub pixel accumulator

    // Determine if bounce or roll

    // If roll, skip low-energy micro-bounce and apply force into rolling motion

    // If bounce, apply basic axes swap

}

// Lookup tables

const fixed_n RANDOM_HORIZONTAL_VX[20] = {
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50    // -0.5
};

/* End of physics.c */