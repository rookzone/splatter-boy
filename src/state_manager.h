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

void set_state(uint8_t state);
void init_state(void);
void update_state(void);
void cleanup_state(void);

#endif // STATE_MANAGER_H_