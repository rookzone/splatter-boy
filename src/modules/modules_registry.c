// modules_registry.c

#include "modules_registry.h"
#include "modules_enabled.h"
#include "../platform.h"

#if MODULE_INPUT_ENABLED
#include "input.h"
#endif
#if MODULE_GRAPHICS_ENABLED
#include "graphics.h"
#include "../tiles/menuFont.h"
#endif
#if MODULE_PHYSICS_ENABLED
#include "physics.h"
#endif
#if MODULE_BALL_ENABLED
#include "ball.h"
#endif
#if MODULE_SOUND_ENABLED
#include "sound.h"
#endif

// Module dependency bits (used only at init-time)
#define MODULE_BIT_INPUT    (1u << 0)
#define MODULE_BIT_GRAPHICS (1u << 1)
#define MODULE_BIT_PHYSICS  (1u << 2)
#define MODULE_BIT_BALL     (1u << 3)
#define MODULE_BIT_SOUND    (1u << 4)

typedef struct {
    const char* name;
    uint8_t bit;
    uint8_t deps;
} ModuleInfo;

static const ModuleInfo MODULES[] = {
    { "input",    MODULE_BIT_INPUT,    0 },
    { "graphics", MODULE_BIT_GRAPHICS, 0 },
    { "physics",  MODULE_BIT_PHYSICS,  0 },
    { "ball",     MODULE_BIT_BALL,     (MODULE_BIT_GRAPHICS | MODULE_BIT_PHYSICS) },
    { "sound",    MODULE_BIT_SOUND,    0 }
};

static uint8_t modules_enabled_mask(void)
{
    uint8_t mask = 0;
#if MODULE_INPUT_ENABLED
    mask |= MODULE_BIT_INPUT;
#endif
#if MODULE_GRAPHICS_ENABLED
    mask |= MODULE_BIT_GRAPHICS;
#endif
#if MODULE_PHYSICS_ENABLED
    mask |= MODULE_BIT_PHYSICS;
#endif
#if MODULE_BALL_ENABLED
    mask |= MODULE_BIT_BALL;
#endif
#if MODULE_SOUND_ENABLED
    mask |= MODULE_BIT_SOUND;
#endif
    return mask;
}

static const char* module_name_from_bit(uint8_t bit)
{
    switch (bit) {
        case MODULE_BIT_INPUT:    return "input";
        case MODULE_BIT_GRAPHICS: return "graphics";
        case MODULE_BIT_PHYSICS:  return "physics";
        case MODULE_BIT_BALL:     return "ball";
        case MODULE_BIT_SOUND:    return "sound";
        default:                  return "unknown";
    }
}

static void modules_show_error(const char* module_name, const char* dep_name)
{
#if MODULE_GRAPHICS_ENABLED
    platform_display_off();
    platform_sprites_8x8();
    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);
    text_clear_screen();
    print_text("MODULE ERROR", 1, 2);
    print_text("module:", 1, 4);
    print_text((char*)module_name, 9, 4);
    print_text("needs:", 1, 5);
    print_text((char*)dep_name, 8, 5);
    platform_show_background();
    platform_show_sprites();
    platform_display_on();
#else
    (void)module_name;
    (void)dep_name;
    platform_display_off();
#endif

    while (1) {
        platform_vsync();
    }
}

static void modules_validate_or_halt(void)
{
    uint8_t enabled = modules_enabled_mask();

    for (uint8_t i = 0; i < (uint8_t)(sizeof(MODULES) / sizeof(MODULES[0])); i++) {
        const ModuleInfo* mod = &MODULES[i];

        if ((enabled & mod->bit) == 0)
            continue;

        uint8_t missing = (uint8_t)(mod->deps & ~enabled);
        if (missing == 0)
            continue;

        // Show the first missing dependency
        for (uint8_t b = 0; b < 8; b++) {
            uint8_t bit = (uint8_t)(1u << b);
            if (missing & bit) {
                modules_show_error(mod->name, module_name_from_bit(bit));
            }
        }
    }
}

void modules_init(void)
{
    modules_validate_or_halt();

#if MODULE_INPUT_ENABLED
    module_input_init();
#endif
#if MODULE_GRAPHICS_ENABLED
    module_graphics_init();
#endif
#if MODULE_PHYSICS_ENABLED
    module_physics_init();
#endif
#if MODULE_BALL_ENABLED
    module_ball_init();
#endif
#if MODULE_SOUND_ENABLED
    module_sound_init();
#endif
}

void modules_update(void)
{
#if MODULE_INPUT_ENABLED
    module_input_update();
#endif
#if MODULE_GRAPHICS_ENABLED
    module_graphics_update();
#endif
#if MODULE_PHYSICS_ENABLED
    module_physics_update();
#endif
#if MODULE_BALL_ENABLED
    module_ball_update();
#endif
#if MODULE_SOUND_ENABLED
    module_sound_update();
#endif
}

void modules_shutdown(void)
{
#if MODULE_BALL_ENABLED
    module_ball_shutdown();
#endif
#if MODULE_SOUND_ENABLED
    module_sound_shutdown();
#endif
#if MODULE_PHYSICS_ENABLED
    module_physics_shutdown();
#endif
#if MODULE_GRAPHICS_ENABLED
    module_graphics_shutdown();
#endif
#if MODULE_INPUT_ENABLED
    module_input_shutdown();
#endif
}

/* End of modules_registry.c */
