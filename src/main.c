// main.c

#include <gb/gb.h>
#include <stdio.h>
#include <stdint.h>



void logKeyPress(uint8_t key){
    uint8_t joyButtonOut = key;
    printf("Joy output %u",joyButtonOut);
}

void main(void) {

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    while(1) {

        uint8_t keys = joypad();

        cls();  // Clear the screen each frame
        printf("Joy: %u", keys); // Print at top-left

        wait_vbl_done(); // Wait for vertical blank
    }



}