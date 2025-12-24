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
#include "../maps/arod_gb.h"

// Font
#include "../tiles/menuFont.h"


void init_title_scene(void)
{

    platform_display_off();
    platform_sprites_8x8();

    set_game_background(arod_gb_map, arod_gb_tiles, arod_gb_TILE_COUNT);

    /**
     * @todo fix up font to use different vram slot...
     * 
     */
    //
    //set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);
    //print_text("Aaron Rodgers Ball Simulator",3 ,15);

    // Turn on our screen, sprites, and BG
    platform_show_background();
    platform_show_sprites();
    platform_display_on();
    
}

void update_title_scene(void)
{
    if (get_key_pressed_down(J_DOWN))
        set_scene(SCENE_GAME);
}

void cleanup_title_scene(void)
{
    platform_hide_sprites(); 
    platform_hide_background();
}

/* End of scenes/scene_game.c */