// state_manager.c

#include <gb/gb.h>

#include "state_manager.h"
#include "game_data.h"

// State headers
#include "states/state_title_screen.h"
#include "states/state_game_screen.h"


void set_state(uint8_t new_state)
{
    if (game.system.current_state != new_state) {

        game.system.current_state = new_state;

        init_state();
    }
}

void buffer_state(uint8_t state)
{
    // Load state data into a copy of init and game_data
}


void init_state(void)
{
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
            init_game_screen();
            break;
        case STATE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown state
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
    switch (game.system.current_state) 
    {
        case STATE_GAME_SCREEN:
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
