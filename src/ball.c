// ball.c

#include "ball.h"
#include "graphics.h"
#include "physics.h"
#include "game_object.h"


GameObject* spawn_ball(uint8_t x, uint8_t y)
{
    // Create generic GameObject
    GameObject* new_object = go_spawn_object(OBJ_BALL);

    // Assign update function to go
    new_object->update = update_ball;
    // New sprite
    new_object->sprite = create_sprite(TILE_BALL);

    // Initialise the ball specific data struct
    init_ball(&new_object->data.ball, &new_object->sprite, x, y);

    // Pass x,y back into generic obj
    new_object->x = x;
    new_object->y = y;

    // Pass x,y back into generic obj
    new_object->x = x;
    new_object->y = y;

    return new_object;
}


void init_ball(Ball* ball, GameSprite* gfx_data, uint8_t ball_x, uint8_t ball_y)
{
    ball->x = ball_x;
    ball->y = ball_y;
    ball->vx = 0;
    ball->vy = 0;
    ball->fractional_x = 0;
    ball->fractional_y = 0;
}


/** @todo
 * DO NOT CHECK COLLISIONS OR DRAW BALLS OFF-SCREEN
 * SWITCH CASE TILE COLLISION CHECK AND CHOOSE INTERACTION
 * Gameboy backgrounds are 360 x 8x8 tiles, a total of 360 tiles.
 * Only 256 unique tiles can be loaded into memory at once.
 */
void update_ball(GameObject* obj) {

    // Grab reference to ball type struct
    Ball* ball = &obj->data.ball;

    handle_ball_pin_collision(ball);

    update_ball_position(ball);

    // After updating, generic object needs updating to match any changes in the ball
    obj->x = ball->x;
    obj->y = ball->y;
}

void reset_all_balls(void)
{
    // Iterate for each ball
    for (uint8_t i = 0; i < MAX_BALLS; i++) {

    GameObject* obj = go_return_ball(i);

    if (obj != NULL){

        Ball* ball = &obj->data.ball;

        // Set initial position based on index
        if (i < 8) {
            ball->x = 10 + i*8;
            ball->y = 20;
        } else {
            ball->x  = 10 + (i-8)*8;
            ball->y = 30;
        }

        // Give random horizontal speed
        ball->vx = RANDOM_HORIZONTAL_VX[i];
        }

    }
}


/* NOT YET REFACTORED TO USE GENERIC GAME OBJECT SYSTEM

void launch_ball(Ball* ball, uint8_t from_x, uint8_t from_y, fixed_n launch_power_x, fixed_n launch_power_y)
{
    ball->vx = 0;
    ball->vy = 0;
    ball->sub_x = 0;
    ball->sub_y = 0;
    ball->x = from_x;
    ball->y = from_y;
    apply_impulse(ball, launch_power_x, launch_power_y);
}
*/
/*



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
    */

/* End of ball.c */