// main.c

#include "scene_manager.h"
#include "debug.h"
#include "game_state.h"
#include "input.h"
#include "graphics.h"
#include "platform.h"
#include "tiles/menuFont.h"

// Forward declarations
void pause_game(void);

void main(void) 
{
    // Load first "game" scene
    set_scene(SCENE_TITLE_SCREEN);

    while (1) {

        // Update state, required every frame
        update_game_state();

        // Pause on start
        if (get_key_pressed_down(J_START)){
            pause_game(); // Update text showing if paused
        }

        update_scene(); // Update game scene

        platform_vsync(); // Await frame end

    }
}

void pause_game(void)
{

    if(game.system.current_scene == SCENE_TITLE_SCREEN)
        return; // Do not pause on title screen

    game.system.paused = !game.system.paused;

    // Load upper case font
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);

    if (game.system.paused == 1){
        print_text("GAME PAUSED...",3 ,8);
    } else if (game.system.paused == 0) {
        print_text("              ",3 ,8);
    }
}

/* End of main.c */