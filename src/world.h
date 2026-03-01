// world.h

#ifndef WORLD_H_
#define WORLD_H_

#include "types.h"

void world_set_collision_tilemap(const unsigned char* tilemap);
const unsigned char* world_get_collision_tilemap(void);

#endif // WORLD_H_
