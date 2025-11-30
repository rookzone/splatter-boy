//main.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include <stdbool.h>

#include "states.h"     // NEW: For state management
#include "memory.h"     // NEW: For object data access
#include "graphics.h"
#include "debug.h"      // Assuming this exists

// Include all necessary tile/map files
#include "tiles/pinballTiles.h"
#include "tiles/pachinkoOneBG.h"

// Forward declaration of Game Boy GBDK routine
void end_step(void);

// Frame counter (can be moved to memory.h/c if needed for multiple states)
uint8_t frame_counter = 0;

void main()
{
    // Set initial state to the main game screen
    change_state(STATE_GAME_SCREEN);
    
    // Main Game Loop
    while (1)
    {
        // Run the current state's logic (init, update, physics, input)
        update_state();

        // Increment frame counter
        frame_counter++;
        
        // GBDK routine to wait for VBlank and refresh screen
        end_step();
    }
}

// GBDK routine that waits for VBlank and synchronizes
void end_step(void)
{
    wait_vbl_done();
}