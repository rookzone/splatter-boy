// scene_manager.c

#include "scene_manager.h"
#include "game_state.h"

// Scene headers
#include "scenes/scene_title_screen.h"
#include "scenes/scene_game.h"
#include "scenes/scene_game2.h"


void set_scene(uint8_t new_scene)
{
    if (game.system.current_scene != new_scene) {

        game.system.current_scene = new_scene;

        init_scene();
    }
}


void init_scene(void)
{
    switch (game.system.current_scene) 
    {
        case SCENE_GAME:
            clear_game_state(&game); // Wipe game state for new load
            init_game_scene(); // Init game screen code in scenes/
            break;
        case SCENE_GAME2:
            clear_game_state(&game); // Wipe game state for new load
            init_game2_scene(); // Init game screen code in scenes/
            break;
        case SCENE_TITLE_SCREEN:
            clear_game_state(&game);
            break;

        default:
            // Handle unknown scene - back to title?zx
            break;
    }
}

void update_scene(void)
{
    if (game.system.paused == 1)
        return;

    switch (game.system.current_scene) 
    {
        case SCENE_GAME:
            update_game_scene();
            break;
        case SCENE_GAME2:
            update_game2_scene();
            break;
        case SCENE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown scene
            break;
    }
}

void cleanup_scene(void)
{
    uint8_t current_scene = game.system.current_scene;

    switch (current_scene) 
    {
        case SCENE_GAME:
            // code
            break;
        case SCENE_GAME2:
            // code
            break;
        case SCENE_TITLE_SCREEN:
            //CODE
            break;

        default:
            // Handle unknown scene
            break;
    }
}

/* End of scene_manager.c */