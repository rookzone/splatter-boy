// state_manager.h

#ifndef STATE_MANAGER_H_
#define STATE_MANAGER_H_

#include "custom_types.h"

void set_state(uint8_t state);
void init_state(void);
void update_state(void);
void cleanup_state(void);

#endif // STATE_MANAGER_H_