// game_data.h

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include "types.h"
#include <stdint.h>

extern GameState game;

// Clean up the state ready for a new setup
void clear_game_data(GameState* game);


#endif // GAME_DATA_H_