// main.c

#include "state_manager.h"
#include "debug.h"
#include "game_data.h"

void main(void) 
{
    set_state(STATE_GAME_SCREEN); 

    while (1) {
        update_state(); // Updates current game
    }
}

/* End of main.c */