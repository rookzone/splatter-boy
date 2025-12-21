// states/state_game_screen.c

#include <stdio.h>

#include "state_game_screen.h"

#include "../platform/gb/platform.h"

#include "../state_manager.h" // State manager for switching states
#include "../game_object.h" // GameObject manager
#include "../game_data.h"

// Game elements
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../debug.h"
#include "../input.h"

// Map assets
#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

// Font

#include "../tiles/menuFont.h"


void init_game_screen(void)
{
    platform_display_off();
    platform_sprites_8x8();

    // Load in the sprite and background we want
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(game1Map, PanchinkoTiles);

    // Spawn some balls in
    for (uint8_t i = 0; i < NUM_BALLS; i++) {

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
        ball->physics.vx = RANDOM_HORIZONTAL_VX[i];
    }

    // Turn on our screen, sprites, and BG
    platform_show_background();
    platform_show_sprites();
    platform_display_on();
}

void update_game_screen(void)
{

    if (get_key_down(J_LEFT))
        launch_ball(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);

    if (get_key_down(J_RIGHT))
        launch_ball_random(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);

    if (get_key_down(J_UP))
        reset_all_balls();

    if (get_key_down(J_DOWN))
        set_state(STATE_GAME2_SCREEN);

    if(get_key_down(J_SELECT) && get_key_down(J_A)) {
        debug_print_page_graphics();
    }


    // Update all balls
    go_update_all_balls();

    // Draw all balls
    go_draw_all_balls();
    
}

void cleanup_game_screen(void)
{
    platform_hide_sprites(); 
    platform_hide_background();
}

/* End of states/state_game_screen.c */