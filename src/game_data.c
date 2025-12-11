// game_data.c

#include "game_data.h"
#include "graphics.h"
#include <string.h>
#include <stdio.h>

GameState game;


void clear_game_data(GameState* game)
{
    DISPLAY_OFF;

    gr_hide_all_sprites();

    // Wipe GameObjects and Graphics.
    memset(&game->objects, 0, sizeof(ObjectManager));
    memset(&game->graphics, 0, sizeof(Graphics));

    // Preserve system as this holds state information.
}

/* End of game_data.c */