// state_manager.c

#include <gb/gb.h>

#include "state_manager.h"
#include "game_data.h"

// State headers
#include "states/state_title_screen.h"
#include "states/state_game_screen.h"
#include "states/state_game2_screen.h"


void set_state(uint8_t new_state)
{
    if (game.system.current_state != new_state) {

        game.system.current_state = new_state;

        init_state();
    }
}


void init_state(void)
{
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
            clear_game_data(&game); // Wipe game state for new load
            init_game_screen(); // Init game screen code in states/
            break;
        case STATE_GAME2_SCREEN:
            clear_game_data(&game); // Wipe game state for new load
            init_game2_screen(); // Init game screen code in states/
            break;
        case STATE_TITLE_SCREEN:
            clear_game_data(&game);
            break;

        default:
            // Handle unknown state - back to title?zx
            break;
    }
}

void update_state(void)
{
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
            update_game_screen();
            break;
        case STATE_GAME2_SCREEN:
            update_game2_screen();
            break;
        case STATE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown state
            break;
    }
    
}

void cleanup_state(void)
{
    uint8_t current_state = game.system.current_state;

    switch (current_state) 
    {
        case STATE_GAME_SCREEN:
            // code
            break;
        case STATE_GAME2_SCREEN:
            // code
            break;
        case STATE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown state
            break;
    }
}

/* End of state_manager.c */