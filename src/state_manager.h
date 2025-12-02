/**
 * @file state_manager.h
 * @brief Manages the high-level game state machine.
 * * @details This module provides the public interface for initializing, 
 * updating, and cleaning up the current game state (e.g., Title Screen, Game Screen).
 * It acts as a central switchboard for the main game loop logic.
 * * @defgroup StateModule Game State Management
 */

#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include "custom_types.h"

// === STATES ===

#define STATE_GAME_SCREEN 1
#define STATE_TITLE_SCREEN 2
#define STATE_SCORE_SCREEN 3
#define STATE_DEMO_SCREEN 4


void set_state(uint8_t state);

// Init state in memory, set_state will check if state is buffered and reference those values
// This may be used to reduce load times... !!! NOT IMPLEMENTED !!!
void buffer_state(uint8_t state);

void init_state(void);

void update_state(void);

void cleanup_state(void);

#endif // STATE_MANAGER_H_