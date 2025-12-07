// main.c

#include <gb/gb.h>
#include "game_data.h"
#include "state_manager.h"

void main(void) 
{
    set_state(STATE_GAME_SCREEN); 

    while (1) {

        update_state();

    }
}

/* End of main.c */