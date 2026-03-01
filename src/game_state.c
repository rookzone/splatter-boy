// game_state.c

#include "game_state.h"
#include "modules/graphics.h"
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

    hide_all_sprites();

    // Wipe GameObjects and Graphics.
    memset(&game->objects, 0, sizeof(ObjectManager));
    memset(&game->graphics, 0, sizeof(Graphics));
    memset(&game->world, 0, sizeof(World));

}

void update_game_state(void)
{
    // Advance system time (input is handled by the input module)
    game.system.system_time++;
}

/* End of game_data.c */
