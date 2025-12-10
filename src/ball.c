// ball.c

#include "ball.h"
#include "graphics.h"
#include "physics.h"
#include "game_object.h"


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


void launch_ball(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y)
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