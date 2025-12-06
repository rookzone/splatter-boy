/**
 * @file pins.h
 * @brief Provides functionality for pin Game Objects.
 * * @details This module provides functions to initialise pins.
 * It can also provide functions for any pin related logic and actions.
 * * @defgroup Game Object
 */
#ifndef PINS_H_
#define PINS_H_

#include "custom_types.h"

// Pass an array of type Pin to initialise the pins.
void init_pin(Pin* pin, GameSprite* gfx_data, uint8_t pin_x, uint8_t pin_y);

// Pass a pin to initialise without graphics (for pins created using background)
void init_background_pin(Pin* pin, uint8_t pin_x, uint8_t pin_y);

#endif // PINS_H_