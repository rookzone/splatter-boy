// debug.h

#ifndef DEBUG_H_
#define DEBUG_H_

#include "custom_types.h"


// Pauses game - shows screen with values in GameState game (game_data.c/.h)
void debug_print_data(void);

//
uint16_t get_execution_time(void);

#endif // DEBUG_H_