// states/state_game_screen.c

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>

#include "../game_data.h"
#include "../game_object.h"
#include "../graphics.h"
#include "../physics.h"
#include "../ball.h"
#include "../pins.h"

#include "state_game_screen.h"

#include "../tiles/pachinkoTiles.h"
#include "../maps/pachinkoMap.h"

void init_game_screen(void)
{
    DISPLAY_OFF;
    SPRITES_8x8;
    
    // Initialize the Object Manager (Resets counters)
    go_init_manager(); 
    
    // Reset the sprite counter
    game.graphics.next_sprite_id = 0;

    // Load in the sprite and background we want
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(PachinkoMap, PanchinkoTiles);

    // Spawn some balls in
    for (uint8_t i = 0; i < MAX_BALLS; i++) {
        
        // Create new ball object
        GameObject* object = go_spawn_object(OBJ_BALL);
        
        if (object) {
            // Grab reference to ball type
            Ball* ball = &object->data.ball;

            // Set initial position based on index logic
            if (i < 8) {
                ball->x = 10 + i*8;
                ball->y = 20;
            } else {
                ball->x = 10 + (i-8)*8;
                ball->y = 30;
            }

            // Give random horizontal speed
            ball->vx = RANDOM_HORIZONTAL_VX[i];
            
            // Sync ball position to generic
            object->x = ball->x;
            object->y = ball->y;
        }
    }

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}

void update_game_screen(void)
{
    game.system.previous_keys = game.system.keys;
    game.system.keys = joypad();

    // INPUT MISSING

    // Update all game objects
    // All ball update logic in associated source files. Check ball.c for ball update
    go_update_all();

    // Draw all game objects
    go_draw_all();
        
    vsync();
}

void cleanup_game_screen(void)
{
    HIDE_SPRITES; 
    HIDE_BKG;
}











