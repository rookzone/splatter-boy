// states/state_game_screen.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

#include "../game_data.h"
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../pins.h"

#include "state_game_screen.h"

#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

void init_game_screen(void)
{
    // The previous init_game_state() content goes here
    DISPLAY_OFF;
    SPRITES_8x8;
    HIDE_BKG;
    HIDE_SPRITES;

    game.graphics.next_sprite_id = 0;

    set_sprite_sheet(PanchinkoTiles);
    set_game_background(PachinkoMap, PanchinkoTiles);

    // Create two rows of balls
    for (uint8_t i = 0; i < NUM_BALLS/2; i++) {
        uint8_t x = 10 + i*8;
        uint8_t y = 20;
        init_ball(&game.objects.balls.list[i], &game.objects.balls.graphics[i], x, y);
        game.objects.balls.list[i].vx = RANDOM_HORIZONTAL_VX[i];
    }

    for (uint8_t i = 8; i < NUM_BALLS; i++) {
        uint8_t x = 10 + i*8;
        uint8_t y = 30;
        init_ball(&game.objects.balls.list[i], &game.objects.balls.graphics[i], x, y);
        game.objects.balls.list[i].vx = RANDOM_HORIZONTAL_VX[i];
    }

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
}

/** @todo
 * DO NOT CHECK COLLISIONS OR DRAW BALLS OFF-SCREEN
 * SWITCH CASE TILE COLLISION CHECK AND CHOOSE INTERACTION
 * Gameboy backgrounds are 360 x 8x8 tiles, a total of 360 tiles.
 * Only 256 unique tiles can be loaded into memory at once.
 */
void update_game_screen(void)
{
    // === INPUT ===

    game.system.keys = joypad();

    if ((game.system.keys & J_LEFT) && !(game.system.previous_keys & J_LEFT)) {

        Ball* ball_to_launch = find_lowest_ball(game.objects.balls.list, NUM_BALLS);

        if (ball_to_launch != NULL) {
            // Assumes LAUNCH_FORCE_X, LAUNCH_FORCE_Y, FIXED_HALF are visible
            launch_ball(ball_to_launch, 0, 90, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y + FIXED_HALF);
        }
    }

    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_balls(game.objects.balls.list, NUM_BALLS);
    }
    
    // === PHYSICS ===

    for (uint8_t i = 0; i < NUM_BALLS; i++) {
        
        uint8_t ball_center_x = game.objects.balls.list[i].x + TILE_HALF_WIDTH;
        uint8_t ball_center_y = game.objects.balls.list[i].y + TILE_HALF_WIDTH;

        uint8_t col = PIXEL_TO_GRID(ball_center_x);
        uint8_t row = PIXEL_TO_GRID(ball_center_y);

        uint16_t tilemap_index = GET_TILE_INDEX(col, row);

        if (game.graphics.active_background_tilemap[tilemap_index] == PIN_TILE_ID) {
            
            Pin virtual_pin;
            virtual_pin.x = GRID_TO_PIXEL(col);
            virtual_pin.y = GRID_TO_PIXEL(row);

            handle_ball_pin_collision(&game.objects.balls.list[i], &virtual_pin);
        }
    
        // Update position and draw
        update_ball_position(&game.objects.balls.list[i]);
        
        DRAW_SPRITE(game.objects.balls.list[i].game_sprite, game.objects.balls.list[i].x, game.objects.balls.list[i].y);
    }
    
    // === END STEP / VSYNC ===
    game.system.previous_keys = game.system.keys; // Place value of keys into previous_keys
    vsync();
}

void cleanup_game_screen(void)
{

    HIDE_SPRITES; 
    HIDE_BKG;
}
