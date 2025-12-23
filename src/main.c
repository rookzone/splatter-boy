// main.c

#include "scene_manager.h"
#include "debug.h"
#include "game_state.h"
#include "input.h"
#include "graphics.h"
#include "platform.h"
#include "tiles/menuFont.h"

void print_pause_text(void); // prototype

void main(void) 
{
    set_scene(SCENE_GAME);
    game.system.paused = 0;

    while (1) {

        // Refresh key press values
        game.system.previous_keys = game.system.keys;
        game.system.keys = platform_get_input();

        if (get_key_pressed_down(J_START)){
            game.system.paused = !game.system.paused;
            print_pause_text();
        }

        update_scene();

        platform_vsync();

    }
}

void print_pause_text(void)
{
    // Load upper case font
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);

    if (game.system.paused == 1){
        print_text("GAME PAUSED...",3 ,8);
    } else if (game.system.paused == 0) {
        print_text("              ",3 ,8);
    }
}

/* End of main.c */