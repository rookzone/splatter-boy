// states/state_game_screen.c

#include <gb/gb.h>
#include <stdio.h>

#include "state_game2_screen.h"

#include "../state_manager.h" // State manager for switching states
#include "../game_object.h" // GameObject manager
// Game elements
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../pins.h"

// Map assets
#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

void init_game2_screen(void)
{
    DISPLAY_OFF;
    SPRITES_8x8;

    // Load in the sprite and background we want
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(PachinkoMap, PanchinkoTiles);

    // Spawn some balls in
    for (uint8_t i = 0; i < NUM_BALLS; i++) {

        //GameObject* ball;

        /*
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
        ball->physics.vx = RANDOM_HORIZONTAL_VX[i];
        */
    }

    // Turn on our screen, sprites, and BG
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}

void update_game2_screen(void)
{
    game.system.previous_keys = game.system.keys;
    game.system.keys = joypad();

    // INPUT FUNCTIONS MISSING, ACCESSING GAMESTATE DIRECTLY

    if (game.system.keys & J_LEFT && !(game.system.previous_keys & J_LEFT)){
        launch_ball(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);
    }

    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_all_balls();
    }
    
    // Update all balls
    go_update_all_balls();

    // Draw all balls
    go_draw_all_balls();
        
    vsync();
}

void cleanup_game2_screen(void)
{
    // Memset?
    HIDE_SPRITES; 
    HIDE_BKG;
}

/* End of states/state_game_screen.c */