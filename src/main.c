// main.c

#include "state_manager.h"
#include "debug.h"
#include "game_data.h"
#include "platform/gb/platform.h"

void main(void) 
{
    set_state(STATE_GAME_SCREEN); 

    while (1) {
        // Refresh key press values
        game.system.previous_keys = game.system.keys;
        game.system.keys = platform_get_input();

        update_state(); // Updates current level
        
        // Await new frame
        platform_vsync();
    }
}

/* End of main.c */