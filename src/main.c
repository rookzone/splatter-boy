// main.c

#include "state_manager.h"
#include "debug.h"
#include "game_data.h"
#include "input.h"
#include "graphics.h"
#include "platform/gb/platform.h"
#include "tiles/menuFont.h"

int game_paused = 0;

void print_pause_text()
{
    // Load upper case font
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);

    if (game_paused == 1){
        print_text("GAME PAUSED...",3 ,8);
    } else if (game_paused == 0) {
        print_text("              ",3 ,8);
    }
}


void main(void) 
{
    set_state(STATE_GAME_SCREEN); 
    
    while (1) {

        // Refresh key press values
        game.system.previous_keys = game.system.keys;
        game.system.keys = platform_get_input();

        if (get_key_down(J_START)){
            game_paused = !game_paused;
            print_pause_text();
            continue;
        }

        if (!game_paused) {
            update_state(); 
        } else {
            
        }
        
        // Await new frame
        platform_vsync();

    }
}

/* End of main.c */