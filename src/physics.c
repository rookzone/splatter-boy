// physics.c

#include "graphics.h"
#include "physics.h"
#include "game_state.h"

void update_ball_position(GameObject* ball)
{
    ball->physics.vy += GRAVITY;

    if (ball->physics.vy > MAX_SPEED)
        ball->physics.vy = MAX_SPEED;
    
    // Add our velocity to the fixed number fraction left over from last frame
    ball->physics.fractional_vx += ball->physics.vx;
    ball->physics.fractional_vy += ball->physics.vy;

    // Convert fixed value back to signed integer and apply to screen position
    ball->transform.x += (int8_t)(ball->physics.fractional_vx >> FIXED_SHIFT);
    ball->transform.y += (int8_t)(ball->physics.fractional_vy >> FIXED_SHIFT);
    
    // Zero off the left-hand byte to leave us with the decimal
    ball->physics.fractional_vx &= 0xFF;
    ball->physics.fractional_vy &= 0xFF;

}

void apply_impulse(GameObject* obj, fixed_t impulse_magnitude_x, fixed_t impulse_magnitude_y)
{
    obj->physics.vx += impulse_magnitude_x;
    obj->physics.vy += impulse_magnitude_y;
}

void check_ball_pin_collision(GameObject* ball)
{
    
    // FILTER: Only handle collision if ball is moving downward
    if (ball->physics.vy <= 0)
        return;

    // Get ball's bottom-middle point
    uint8_t ball_bottom_x = ball->transform.x + TILE_HALF_WIDTH;
    uint8_t ball_bottom_y = ball->transform.y  + TILE_WIDTH;

    // Find which 8x8 tile the ball's bottom is in
    uint8_t col = PIXEL_TO_GRID(ball_bottom_x);
    uint8_t row = PIXEL_TO_GRID(ball_bottom_y);

    // FILTER: Outside screen, do not collide
    if (col >= BACKGROUND_WIDTH_TILES || row >= BACKGROUND_HEIGHT_TILES)
        return;

    // Grab the tiles index value
    uint16_t tilemap_index = GET_TILE_INDEX(col, row);

    // FILTER: Check the tile in the map at that index, if it's a pin tile we need to collide
    if (game.graphics.active_background_tilemap[tilemap_index] != PIN_TILE_ID)
        return;
            
    // Get coords of pin tile
    uint8_t pin_tile_x = GRID_TO_PIXEL(col);
    uint8_t pin_tile_y = GRID_TO_PIXEL(row);

    // Calculate horizontal distances to determine if ball will hit pin
    uint8_t pin_center_x = pin_tile_x + TILE_HALF_WIDTH;
    int8_t distance_x = ball_bottom_x - pin_center_x;
    
    // FILTER: check if ball is within horizontal collision bounds
    if (distance_x < -(PIN_HALF_WIDTH) || distance_x > (PIN_HALF_WIDTH))
        return;

    // Calculate vertical distance so collision only happens from above
    uint8_t pin_center_y = pin_tile_y + PIN_HALF_WIDTH;
    int8_t distance_y = ball_bottom_y - pin_center_y;
    
    // FILTER: check if ball is within verticle collision bounds (ignore collision from below)
    if (distance_y < 0 || distance_y > TILE_HALF_WIDTH)
        return;

    // Zero fractional velocity
    ball->physics.fractional_vy = 0;

    if (ball->physics.vy > FIXED_QUARTER) { 
        // === BOUNCE ===
        
        // Vertical bounce (50% energy)
        ball->physics.vy = -(ball->physics.vy >> 1);
        
        // If ball moving right and hits left side (distance_x < 0), bounce left
        // If ball moving left and hits right side (distance_x > 0), bounce right
        if ((ball->physics.vx > 0 && distance_x < 0) || (ball->physics.vx < 0 && distance_x > 0)) {
            // Hit the "wrong" side - reverse direction with damping
            ball->physics.vx = -(ball->physics.vx >> 1);
        } else {
            // Hit the "correct" side - keep direction but add deflection force
            ball->physics.vx = (ball->physics.vx >> 1);
            
            // Add a nudge based on offset from center
            if (distance_x > 0) {
                ball->physics.vx += FIXED_EIGHTH;  // nudge right
            } else if (distance_x < 0) {
                ball->physics.vx -= FIXED_EIGHTH;  // nudge left
            } else {
                // Dead center hit - give it a small nudge to break symmetry
                ball->physics.vx += (ball->transform.y & 1) ? FIXED_EIGHTH : -FIXED_EIGHTH;
            }
        }
        
    } else { 

        // === ROLL ===
        
        ball->physics.vy = 0;
        
        // Shift distance_x to scale it down
        ball->physics.vx += distance_x;
        
        // Clamp horizontal speed
        if (ball->physics.vx > MAX_ROLL_SPEED) ball->physics.vx = MAX_ROLL_SPEED;
        else if (ball->physics.vx < -MAX_ROLL_SPEED) ball->physics.vx = -MAX_ROLL_SPEED;

    }
}

/* End of physics.c */