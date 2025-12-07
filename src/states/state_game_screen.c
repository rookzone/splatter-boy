// states/state_game_screen.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

#include "../game_data.h"
#include "../game_object.h"
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../pins.h"

#include "state_game_screen.h"

#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

void init_game_screen(void)
{
    DISPLAY_OFF;
    SPRITES_8x8;
    
    // Initialize the Object Manager (Resets counters)
    go_init_manager(); 
    
    // Reset the sprite counter
    game.graphics.next_sprite_id = 0;

    // Load in the sprite and background we want
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(PachinkoMap, PanchinkoTiles);

    // Spawn some balls in
    for (uint8_t i = 0; i < MAX_BALLS; i++) {

        GameObject* ball;

        // Set initial position based on index
        if (i < 8) {
            uint8_t ball_x = 10 + i*8;
            uint8_t ball_y = 20;
            ball = spawn_ball(ball_x, ball_y);
        } else {
            uint8_t ball_x  = 10 + (i-8)*8;
            uint8_t ball_y = 30;
            ball = spawn_ball(ball_x, ball_y);
        }

        // Give random horizontal speed
        ball->data.ball.vx = RANDOM_HORIZONTAL_VX[i];
    }

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}

void update_game_screen(void)
{
    game.system.previous_keys = game.system.keys;
    game.system.keys = joypad();

    // INPUT MISSING

    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_all_balls();
    }
    
    // Update all game objects
    // All ball update logic in associated source files. Check ball.c for ball update
    go_update_all();

    // Draw all game objects
    go_draw_all();
        
    vsync();
}

void cleanup_game_screen(void)
{
    HIDE_SPRITES; 
    HIDE_BKG;
}