// world.c

#include "world.h"
#include "game_state.h"

void world_set_collision_tilemap(const unsigned char* tilemap)
{
    game.world.collision_tilemap = tilemap;
}

const unsigned char* world_get_collision_tilemap(void)
{
    return game.world.collision_tilemap;
}

/* End of world.c */
