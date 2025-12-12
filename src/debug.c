// debug.c

#include <gbdk/emu_debug.h>

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include "debug.h"
#include "game_data.h"

// Helper to clear screen and reset cursor
void debug_cls(void) {
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // Primitive but effective flush
    gotoxy(0, 0);
}

void debug_print_page_system(void) {
    debug_cls();
    printf("== SYSTEM STATE ==\n\n");
    printf("State ID:  %u\n", game.system.current_state); //
    printf("Keys:      0x%x\n", game.system.keys); //
    printf("Prev Keys: 0x%x\n", game.system.previous_keys); //

}

void debug_print_page_objects(void) {
    debug_cls();
    printf("== OBJECT POOL ==\n\n");
    printf("Total Objs: %u/%u\n", game.objects.total_count, MAX_GAME_OBJECTS); //
    printf("Balls:      %u/%u\n", game.objects.ball_count, MAX_BALLS); //
    
    // Show details of the first 3 balls if they exist
    printf("\n-- First Balls --\n");
    for(uint8_t i=0; i<3; i++) {
        if(i < game.objects.ball_count) {
            GameObject* b = go_return_ball(i);
            if(b) {
                // Display position and velocity components
                printf("#%u X:%u Y:%u\n", i, b->transform.x, b->transform.y); 
                printf("  VX:%d FY:%d\n", b->physics.vx, b->physics.vy); 
            }
        }
    }
}

void debug_print_page_graphics(void) {
    debug_cls();
    printf("== GRAPHICS ==\n\n");
    printf("Nxt Spr ID: %u\n", game.graphics.next_sprite_slot); //
    printf("Spr Count:  %u\n", game.graphics.sprite_count); //
    
    // Show pointer addresses
    printf("Map Ptr:    %x\n", (uint16_t)game.graphics.active_background_tilemap); //
    printf("Tile Ptr:   %x\n", (uint16_t)game.graphics.active_background_tile_sheet); //
}

/* End of debug.c */