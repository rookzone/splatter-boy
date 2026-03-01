// grid.h
// Tile/grid helper macros shared across modules.

#ifndef GRID_H_
#define GRID_H_

#include <stdint.h>

// Convert Pixel coordinate to Grid coordinate (Divide by 8)
#define PIXEL_TO_GRID(x) ((uint8_t)((x) >> 3))

// Convert Grid coordinate to Pixel coordinate (Multiply by 8)
#define GRID_TO_PIXEL(x) ((uint8_t)((x) << 3))

// Get tilemap Array Index from Col/Row (Row * Width + Col)
#define GET_TILE_INDEX(col, row) ((((uint16_t)(row) << 4) + ((uint16_t)(row) << 2)) + (col))

#endif // GRID_H_
