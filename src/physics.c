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

void handle_ball_pin_collision(Ball* ball)
{
    // Get ball centers
    uint8_t ball_center_x = ball->x + TILE_HALF_WIDTH;
    uint8_t ball_center_y = ball->y + TILE_HALF_WIDTH;

    // Find which 8x8 tile the ball is in
    uint8_t col = PIXEL_TO_GRID(ball_center_x);
    uint8_t row = PIXEL_TO_GRID(ball_center_y);

    // Outside screen, do not collide
    if (col >= BACKGROUND_WIDTH_TILES || row >= BACKGROUND_HEIGHT_TILES)
        return;

    // Grab the tiles index value
    uint16_t tilemap_index = GET_TILE_INDEX(col, row);

    // Check the tile in the map at that index, if it's a pin tile we need to collide
    if (get_game_background_tilemap()[tilemap_index] != PIN_TILE_ID)
        return;
            
    // Get coords of "pin"
    uint8_t pin_x = GRID_TO_PIXEL(col);
    uint8_t pin_y = GRID_TO_PIXEL(row);

    // Calculate distance from center
    uint8_t pin_center_x = pin_x + TILE_HALF_WIDTH;
    int8_t distance_x = ball_center_x - pin_center_x;
    // distance_x < 0 = left side, > 0 = right side
    
    // Outside pin bounds - keep falling
    if (distance_x < -(PIN_HALF_WIDTH+1) || distance_x > (PIN_HALF_WIDTH+1) )
        return;           

    /**
     * @todo NEED TO ADD BACK IN ROLLING LOGIC AS BALL SEEMS TO LIKE SETTLING ON TOP OF PIN
     * a LITTLE NUDGE SHOULD DO IT. THEN GET TO SOMETHING ELSE, IT MIGHT TURN OUT THAT
     * COLLISION AND BOUNCING THIS PRECISE DOES NTO AFFET GAEMPLAY AT ALL!!!
     * 
     */
    
    // don't collide with pins when moving upwards
    if (ball->vy > 0) { // Also make sure ball is moving faster to avoid top of arc collisions

        // Zero fractional velocity
        ball->fractional_y = 0; 

        // Snap ball to top of pin tile
        //ball->y = pin_y - TILE_WIDTH;
        
        // Apply vertical bounce
        ball->vy = -(ball->vy >> 1); // 50% energy retention

        // Apply simple reflection for x
        if (ball->vx > 0 && distance_x < 0 || ball->vx < 0 && distance_x > 0){
            ball->vx = -(ball->vx >> 1);
        }

    }
    
}

// junk from ball collision
/*
    else { // Roll

        ball->vy = 0;
        ball->vx += distance_x;

        // Clamp horizontal speed
        if (ball->vx > MAX_ROLL_SPEED) ball->vx = MAX_ROLL_SPEED;
        else if (ball->vx < -MAX_ROLL_SPEED) ball->vx = -MAX_ROLL_SPEED;
    }
*/

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