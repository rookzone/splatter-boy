// ball.c

#include "ball.h"
#include "graphics.h"
#include "physics.h"
#include "game_object.h"
#include "game_data.h"
#include <stdlib.h>

GameObject* spawn_ball(uint8_t x, uint8_t y) {

    GameObject* obj = go_spawn_object(OBJ_BALL);
    if (obj == NULL) return NULL;
    
    // Configure components
    obj->flags |= (TRANSFORM_ACTIVE | PHYSICS_ACTIVE | RENDERER_ACTIVE);
    
    // Transform
    obj->transform.x = x;
    obj->transform.y = y;
    
    // Physics
    obj->physics.vx = 0;
    obj->physics.vy = 0;
    obj->physics.fractional_vx = 0;
    obj->physics.fractional_vy = 0;
    obj->physics.collision_enabled = 1;
    
    // Render
    GameSprite sprite = create_sprite(TILE_BALL);
    obj->renderer.sprite_index = sprite.sprite_index;
    obj->renderer.tile_index = sprite.tile_index;
    obj->renderer.visible = 1;
    
    return obj;
}


void update_ball(GameObject* obj) {

    // Quick validation
    if (!(obj->flags & PHYSICS_ACTIVE)) return;
    
    // Handle collision (pass GameObject directly)
    handle_ball_pin_collision(obj);
    
    // Update position (pass GameObject directly)
    update_ball_position(obj);
    
}

void reset_all_balls(void) {

    for (uint8_t i = 0; i < game.objects.ball_count; i++) {

        GameObject* obj = go_return_ball(i);

        if (obj == NULL) continue;
        
        // Reset position
        if (i < NUM_BALLS >> 1) {
            obj->transform.x = 10 + i * 8;
            obj->transform.y = 20;
        } else {
            obj->transform.x = 10 + (i - 8) * 8;
            obj->transform.y = 30;
        }
        
        // Reset physics
        obj->physics.vx = RANDOM_HORIZONTAL_VX[i];
        obj->physics.vy = 0;
        obj->physics.fractional_vx = 0;
        obj->physics.fractional_vy = 0;
    }
}

// ball.c

// ... existing includes ...

void launch_ball_random(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t base_power_x, fixed_t base_power_y)
{
    // 1. Reset the ball (same logic as your standard launch)
    ball->physics.vx = 0;
    ball->physics.vy = 0;
    ball->physics.fractional_vx = 0;
    ball->physics.fractional_vy = 0;
    
    ball->transform.x = from_x;
    ball->transform.y = from_y;

    // 2. Calculate Jitter
    // We want a variation of roughly +/- 0.25 (FIXED_QUARTER)
    // 1.0 in your fixed point is 256 (FIXED_SHIFT 8)
    // 0.25 is 64.
    
    // rand() & 0x7F gives a range of 0 to 127.
    // Subtracting 64 shifts this range to -64 to +63.
    // This creates a spread of roughly -0.25 to +0.25 fixed-point units.
    
    fixed_t jitter_x = (rand() & 0x7F) - 100; 
    fixed_t jitter_y = (rand() & 0x7F) - 100; 

    // 3. Apply the Impulse
    // Add the jitter to the base power and apply using your physics engine
    apply_impulse(ball, base_power_x + jitter_x, base_power_y + jitter_y);
}


void launch_ball(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t launch_power_x, fixed_t launch_power_y)
{
    ball->physics.vx = 0;
    ball->physics.vy = 0;
    ball->physics.fractional_vx = 0;
    ball->physics.fractional_vy = 0;
    ball->transform.x = from_x;
    ball->transform.y = from_y;
    apply_impulse(ball, launch_power_x, launch_power_y);
}



GameObject* find_lowest_ball(void)
{
    GameObject* lowest_ball = &game.objects.pool[game.objects.ball_indices[0]];
    uint8_t highest_y = game.objects.pool[game.objects.ball_indices[0]].transform.y;

    for (uint8_t i = 1; i < game.objects.ball_count; i++) {
        // Compare the current ball's y-coordinate with the maximum found so far
        if (game.objects.pool[game.objects.ball_indices[i]].transform.y > highest_y) {
            highest_y = game.objects.pool[game.objects.ball_indices[i]].transform.y;
            lowest_ball = &game.objects.pool[game.objects.ball_indices[i]];
        }
    }

    return lowest_ball;
}

/* End of ball.c */