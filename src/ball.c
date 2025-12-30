// ball.c

#include <stdlib.h>
#include <stdint.h>
#include "ball.h"
#include "graphics.h"
#include "physics.h"
#include "game_object.h"
#include "game_state.h"
#include <gbdk/emu_debug.h>

GameObject* spawn_ball(uint8_t x, uint8_t y) {


    // Pass OBJ_BALL so object manager will put it in ball registry
    GameObject* obj = go_new_game_object(OBJ_BALL);

    if (obj == NULL)
        return NULL;
    
    // Configure components
    obj->flags |= (TRANSFORM_ACTIVE | PHYSICS_ACTIVE | RENDERER_ACTIVE);
    
    // Transform
    obj->transform.x = x;
    obj->transform.y = y;
    
    // Physics
    obj->physics.vx = 0;
    obj->physics.vy = 0;
    obj->physics.position_accumulator_x = 0;
    obj->physics.position_accumulator_y = 0;
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
    
    EMU_PROFILE_BEGIN("COLLISION CHECK ")
    if ((obj->id & COLLISION_FRAME_SKIP) == (game.system.system_time & COLLISION_FRAME_SKIP) )
    {
        check_ball_pin_collision(obj);
    }
    EMU_PROFILE_END("COLLISION CHECK ")

    EMU_PROFILE_BEGIN("UPDATE POSITION VALUES ")
    // Update position
    update_ball_position(obj);
    EMU_PROFILE_END("UPDATE POSITION VALUES ")

    EMU_PROFILE_BEGIN("REDRAW SPRITE ")
    uint8_t x = obj->transform.x + 8;
    uint8_t y = obj->transform.y + 16;
    uint8_t idx = obj->renderer.sprite_index;

    move_sprite(idx, x, y);
    EMU_PROFILE_END("REDRAW SPRITE ")
    
}

// This is for testing purposes
void reset_all_balls(void) {

    for (uint8_t i = 0; i < game.objects.ball_count; i++) {

        GameObject* obj = go_get_ball(i);

        if (obj == NULL)
            continue;
        
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
        obj->physics.position_accumulator_x = 0;
        obj->physics.position_accumulator_y = 0;
    }
}


void launch_ball_random(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t base_power_x, fixed_t base_power_y)
{
    ball->physics.vx = 0;
    ball->physics.vy = 0;
    ball->physics.position_accumulator_x = 0;
    ball->physics.position_accumulator_y = 0;
    
    ball->transform.x = from_x;
    ball->transform.y = from_y;

    fixed_t jitter_x = (rand() & 0x7F) - 100; 
    fixed_t jitter_y = (rand() & 0x7F) - 100; 

    apply_impulse(ball, base_power_x + jitter_x, base_power_y + jitter_y);
}


void launch_ball(GameObject* ball, uint8_t from_x, uint8_t from_y, fixed_t launch_power_x, fixed_t launch_power_y)
{
    ball->physics.vx = 0;
    ball->physics.vy = 0;

    ball->physics.position_accumulator_x = 0;
    ball->physics.position_accumulator_y = 0;

    ball->transform.x = from_x;
    ball->transform.y = from_y;

    apply_impulse(ball, launch_power_x, launch_power_y);
}



GameObject* find_lowest_ball(void)
{
    GameObject* lowest_ball = game.objects.ball_pointers[0];
    // increase in y is decrease in height
    uint8_t highest_y = game.objects.ball_pointers[0]->transform.y;

    for (uint8_t i = 1; i < game.objects.ball_count; i++) {
        // Compare the current ball's y-coordinate with the maximum found so far
        if (game.objects.ball_pointers[i]->transform.y > highest_y) {
            highest_y = game.objects.ball_pointers[i]->transform.y;
            lowest_ball = game.objects.ball_pointers[i];
        }
    }

    return lowest_ball;
}

extern const fixed_t RANDOM_HORIZONTAL_VX[30] = {
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
    -50,    // -0.5
    -50,   // -0.5
    50,    // +0.5
    -50,   // -0.5
    50,    // +0.5
    -50,    // -0.5
    -50,   // -0.5
    50,    // +0.5
    50,    // +0.5
    -50,   // -0.5
    -50   // -0.5
};

/* End of ball.c */