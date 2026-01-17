// scenes/scene_score_screen.c

// A simple score screen implementation. This scene
// demonstrates how to set up a new scene within the
// Splatter Boy engine. It clears the display, loads a
// background and a basic font, prints a few lines of
// placeholder text and returns to the title screen
// when the user presses start or the B button.

#include "scene_score_screen.h"

#include <stdio.h>

#include "../platform.h"
#include "../scene_manager.h"
#include "../game_object.h"
#include "../game_state.h"
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../debug.h"
#include "../input.h"

// Assets for the score screen. We reuse the title
// screen background for now. If you add your own
// background, include its header here instead.
#include "../backgrounds/arod_gb.h"

// Font asset for printing text on screen.
#include "../tiles/menuFont.h"
#include "../constants.h"

// Initialise the score screen. This function is called
// whenever the game switches to the SCENE_SCORE_SCREEN
// scene. It should set up everything the scene needs.
void init_score_scene(void)
{
    // Turn off the display before making any changes.
    platform_display_off();

    // Configure sprite size (8x8 is common for the GB).
    platform_sprites_8x8();

    // Load our background map and tiles. We reuse the
    // Arod Rodgers background from the title screen. The
    // third argument specifies how many tiles to load.
    set_game_background(arod_gb_map, arod_gb_tiles, arod_gb_TILE_COUNT);

    // Load the basic font used throughout the project. See
    // constants.h for BASIC_FONT_TILESET_SIZE.
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);

    // Print a title and a message. print_text takes a
    // string and x/y coordinates in tile units.
    print_text("Score Screen", 3, 5);
    print_text("Press START to return", 3, 7);

    // Show everything on screen. We enable the
    // background, sprites and the display itself.
    platform_show_background();
    platform_show_sprites();
    platform_display_on();
}

// Update the score screen. In this simple implementation
// we just listen for a button press to return to the
// title screen. You could extend this to display the
// player's score or animate elements on screen.
void update_score_scene(void)
{
    // If the start button or the B button is pressed
    // then switch back to the title screen.
    if (get_key_pressed(J_START) || get_key_pressed(J_B))
    {
        set_scene(SCENE_TITLE_SCREEN);
    }
}

// Cleanup the score screen. This hides the background and
// sprites. Any additional cleanup should be done here.
void cleanup_score_scene(void)
{
    platform_hide_sprites();
    platform_hide_background();
}

/* End of scenes/scene_score_screen.c */
