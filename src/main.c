// main.c

#include "scene_manager.h"
#include "debug.h"
#include "game_state.h"
#include "input.h"
#include "graphics.h"
#include "platform.h"
#include "tiles/menuFont.h"

// Forward declarations
void print_pause_text(void);

void main(void) 
{
    // Load first "game" scene
    set_scene(SCENE_GAME);

    while (1) {

        // Pause on start
        if (get_key_pressed_down(J_START)){
            game.system.paused = !game.system.paused;
            print_pause_text(); // Update text showing if paused
        }

        update_scene(); // Update game scene

        platform_vsync(); // Await frame end

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