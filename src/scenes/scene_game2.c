// scenes/scene_game_screen.c

#include "../platform/gb/platform.h"
#include <stdio.h>

#include "scene_game2.h"

#include "../scene_manager.h" // scene manager for switching scenes
#include "../game_object.h" // GameObject manager
#include "../game_state.h"
// Game elements
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../debug.h"
#include "../input.h"

// Map assets
#include "../tiles/pachinkoTiles.h"
#include "../maps/game2Map.h"

// Font

#include "../tiles/menuFont.h"

void init_game2_scene(void)
{
    platform_display_off();
    platform_sprites_8x8();

    // Load in the sprite and background we want
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(game2Map, PanchinkoTiles);

    // Load upper case font
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);
    print_text("a, b, c, d, e, f, g, h, i... 1,2,3,4,5-10...\n what is all this supposed to mean??? !!! --- ...",1 ,1);

    // Spawn some balls in
    for (uint8_t i = 0; i < NUM_BALLS; i++) {

        GameObject* ball;

        
        // Set initial position based on index
        if (i < 8) {
            uint8_t ball_x = 60 + i*8;
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

void update_game2_scene(void)
{

    if (get_key_down(J_LEFT))
        launch_ball(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);

    if (get_key_down(J_RIGHT))
        launch_ball_random(find_lowest_ball(), 10, 80, LAUNCH_FORCE_X, -LAUNCH_FORCE_Y);

    if (get_key_down(J_UP))
        reset_all_balls();

    if (get_key_down(J_DOWN))
        set_scene(SCENE_GAME);

    
    // Update all balls
    go_update_all_balls();

    // Draw all balls
    go_draw_all_balls();
        
}

void cleanup_game2_scene(void)
{
    // Memset?
    platform_hide_sprites(); 
    platform_hide_background();
}

/* End of scenes/scene_game2.c */