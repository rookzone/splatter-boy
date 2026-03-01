// input.c

#include "../game_state.h"
#include "../platform.h"
#include "input.h"


// === MODULE WRAPPER ===

void module_input_init(void)
{
    // No-op for now (kept for module parity)
}

void module_input_update(void)
{
    // Refresh input state (previous -> current)
    game.system.previous_keys = game.system.keys;
    game.system.keys = platform_get_input();
}

void module_input_shutdown(void)
{
    // No-op for now (kept for module parity)
}

//* End of input.c */
