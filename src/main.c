#include <gb/gb.h>
#include <stdio.h>
#include "tiles/SillyTiles.h"  // your header file


enum {
    PLAYER_SPRITE = 0,
    ITEM_SPRITE = 1
} Sprites;

enum {
    TILE_SMILE = 0,
    TILE_HEART = 1
} Tiles;

void main(void) {

    DISPLAY_OFF;      // Turn LCD off during setup to prevent flicker
    SHOW_BKG;         // Enable background
    SPRITES_8x8;      // Set sprite size mode (good practice)
		
	set_sprite_data(0, 2, FunnyTiles); // Load tiles 0-2 into sprite memory

    set_sprite_tile(PLAYER_SPRITE, TILE_SMILE);  // Take tile 0 from sprite memory and display it as sprite number 0
    move_sprite(PLAYER_SPRITE, 95, 75);

    set_sprite_tile(ITEM_SPRITE, TILE_HEART);   // Take tile 1 from sprite memory and display it as sprite number 1
    move_sprite(ITEM_SPRITE, 95 + 8, 75); 

    printf("Testing a GameBoy \ngame.");

    DISPLAY_ON;  // Turn the LCD back on
	SHOW_SPRITES;

	while(1){
        wait_vbl_done();
	}


}
