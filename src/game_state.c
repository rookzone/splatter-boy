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

void update_game_state(void)
{
    // Refresh system values
    game.system.previous_keys = game.system.keys;
    game.system.keys = platform_get_input();
    game.system.system_time = platform_get_sys_time();
}

/* End of game_data.c */