// ball.c

#include "ball.h"
#include "graphics.h"
#include "physics.h"


// Spawning
GameObject* spawn_ball(void){
    
    // Create a new GameObject and list it in the object pool at next available index
    GameObject* new_ball = &game_object_pool[game_object_count];
    new_ball->object_type = OBJ_BALL;
    // ... initialize other generic and ball-specific data

    // 3. Register the index in the ball-type array.
    ball_indices[ball_object_count] = game_object_count;
    ball_object_count++; // Increment the count of active balls
    game_object_count++; // 
    
    return new_ball;

}

void reset_all_balls(Ball* b, uint8_t count)
{
    for (uint8_t i = 0; i < count/2; i++) {
        b[i].x = 10 + i*8;
        b[i].y = 20;
        b[i].vx = 0;
        b[i].vy = 0;

        DRAW_SPRITE(b[i].game_sprite,b[i].x,b[i].y);

        b[i].vx = RANDOM_HORIZONTAL_VX[i];
    }
    
    for (uint8_t i = 8; i < count; i++) {
        b[i].x = 20 + i*8;
        b[i].y = 30;
        b[i].vx = 0;
        b[i].vy = 0;

        DRAW_SPRITE(b[i].game_sprite,b[i].x,b[i].y);

        b[i].vx = RANDOM_HORIZONTAL_VX[i];
    }
    
}

void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y)
{
    ball->x = ball_x;
    ball->y = ball_y;
    ball->vx = 0;
    ball->vy = 0;
    ball->sub_x = 0;
    ball->sub_y = 0;
    
    *gfx_data = create_sprite(TILE_BALL);
    ball->game_sprite = gfx_data;
}

void reset_ball(Ball* ball)
{
    ball->vx = 0;
    ball->vy = 0;
    ball->sub_x = 0;
    ball->sub_y = 0;
}


Ball* find_lowest_ball(Ball* balls, uint8_t count)
{
    if (count == 0) {
        return NULL; // Return NULL if the array is empty
    }

    Ball* lowest_ball = &balls[0];
    uint8_t highest_y = balls[0].y;

    for (uint8_t i = 1; i < count; i++) {
        // Compare the current ball's y-coordinate with the maximum found so far
        if (balls[i].y > highest_y) {
            highest_y = balls[i].y;
            lowest_ball = &balls[i];
        }
    }

    return lowest_ball;
}

void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y)
{
    // Reset ball velocity and sub-pixel accumulator
    reset_ball(ball);

    // Move ball to screen position
    ball->x = from_x;
    ball->y = from_y;

    apply_impulse(ball, launch_power_x, launch_power_y);

}

// end ball.c