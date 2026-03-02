// game_state.c

#include "game_state.h"
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

    // Clear any sprites from the screen (core should not depend on graphics module)
    for (uint8_t i = 0; i < NUM_HW_SPRITES; i++) {
        platform_move_sprite(i, 0, 0);
    }

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
