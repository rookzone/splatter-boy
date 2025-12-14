// states/state_game_screen.c

#include <stdio.h>

#include "state_game_screen.h"

#include "../state_manager.h" // State manager for switching states
#include "../game_object.h" // GameObject manager
// Game elements
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../pins.h"
#include "../debug.h"

// Map assets
#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

// Font

#include "../tiles/menuFont.h"

#include <gb/gb.h>


void init_game_screen(void)
{
    DISPLAY_OFF;
    SPRITES_8x8;

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
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    // Load upper case font
    set_active_font_upper_case(menuFont, 26);
    print_text("AARON RODGERS");
}

void update_game_screen(void)
{
    game.system.previous_keys = game.system.keys;
    game.system.keys = joypad();

    // INPUT FUNCTIONS MISSING, ACCESSING GAMESTATE DIRECTLY

    if (game.system.keys & J_LEFT && !(game.system.previous_keys & J_LEFT)){
        launch_ball(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);
    }

    if (game.system.keys & J_RIGHT && !(game.system.previous_keys & J_RIGHT)){
            launch_ball_random(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);
    }


    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_all_balls();
    }

    if ((game.system.keys & J_DOWN) && !(game.system.previous_keys & J_DOWN)) {
        set_state(STATE_GAME2_SCREEN);
    }

    if((game.system.keys & J_SELECT) && (game.system.keys & J_A) && !(game.system.previous_keys & (J_SELECT || J_A))) {
        debug_print_page_graphics();
    }


    // Update all balls
    go_update_all_balls();

    // Draw all balls
    go_draw_all_balls();
        
    vsync();
}

void cleanup_game_screen(void)
{
    HIDE_SPRITES; 
    HIDE_BKG;
}

/* End of states/state_game_screen.c */