// scenes/scene_game_screen.c

#include "scene_title_screen.h"
#include <stdio.h>

#include "../platform.h"

#include "../scene_manager.h" // scene manager for switching scenes
#include "../game_object.h" // GameObject manager
#include "../game_state.h"

// Game elements
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../debug.h"
#include "../input.h"

// background
#include "../backgrounds/arod_gb.h"

// Font
#include "../tiles/menuFont.h"

// Menu options
#define START_GAME  1
#define CREDITS     2

// Stores menu selection (default start game)
uint8_t option_selected = START_GAME;


void init_title_scene(void)
{

    platform_display_off();
    platform_sprites_8x8();

    set_game_background(arod_gb_map, arod_gb_tiles, arod_gb_TILE_COUNT);

    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);
    print_text("Aaron\nRodgers\nBall\nSimulator",1 ,3);
    print_text(">start game...",3 ,15);
    print_text(" credits",3 ,16);

    // Turn on our screen, sprites, and BG
    platform_show_background();
    platform_show_sprites();
    platform_display_on();
    
}

void update_title_scene(void)
{
    if (get_key_pressed(J_START) && option_selected == START_GAME)
        set_scene(SCENE_GAME);

    if (get_key_pressed(J_DOWN)){

        print_text(" start game...",3 ,15);
        // Add blinking cursor...
        //if (!(game.system.system_time & COLLISION_FRAME_SKIP))
            print_text(">credits",3 ,16);
        //else

        option_selected = CREDITS;
    }

    if (get_key_pressed(J_UP)){
        print_text(">start game...",3 ,15);
        print_text(" credits",3 ,16);
        option_selected = START_GAME;
    }


}

void cleanup_title_scene(void)
{
    platform_hide_sprites(); 
    platform_hide_background();
}

/* End of scenes/scene_game.c */