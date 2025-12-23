// game_data.c

#include "game_state.h"
#include "graphics.h"
#include <string.h>
#include <stdio.h>
#include "platform.h"

GameState game;

/**
 * @todo Allow a mask to be passed for fine control over
 * any parts of the state that should persist
 */
void clear_game_state(GameState* game)
{
    platform_display_off();

    gr_hide_all_sprites();

    // Wipe GameObjects and Graphics.
    memset(&game->objects, 0, sizeof(ObjectManager));
    memset(&game->graphics, 0, sizeof(Graphics));

}

/* End of game_data.c */