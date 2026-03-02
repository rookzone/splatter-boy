// scenes/scene_demo.c

#include <stdio.h>

#include "scene_demo.h"

#include "../platform.h"
#include "../scene_manager.h"
#include "../game_state.h"

// Modules
#include "../modules/graphics.h"
#include "../modules/physics.h"
#include "../modules/ball.h"
#include "../modules/input.h"
#include "../events.h"
#include "../modules/modules_enabled.h"
#if MODULE_SOUND_ENABLED
#include "../modules/sound.h"
#endif

// Assets
#include "../tiles/pachinkoTiles.h"
#include "../backgrounds/game1Map.h"
#include "../tiles/pinballTiles.h"
#include "../tiles/menuFont.h"

static GameObject* demo_player = NULL;
static uint16_t demo_score = 0;
static uint16_t demo_score_prev = 0xFFFF;
static uint8_t demo_charge = 0;
static uint8_t demo_charge_prev = 0xFF;

static void demo_score_to_str(uint16_t value, char* out)
{
    // Fixed 5-digit format for easy overwrite
    out[0] = (char)('0' + ((value / 10000u) % 10u));
    out[1] = (char)('0' + ((value / 1000u) % 10u));
    out[2] = (char)('0' + ((value / 100u) % 10u));
    out[3] = (char)('0' + ((value / 10u) % 10u));
    out[4] = (char)('0' + (value % 10u));
    out[5] = '\0';
}

static void demo_draw_ui(void)
{
    char score_text[6];
    demo_score_to_str(demo_score, score_text);
    print_text("SCORE", 1, 1);
    print_text(score_text, 8, 1);
}

void init_demo_scene(void)
{
    platform_display_off();
    platform_sprites_8x8();

    set_sprite_sheet(PinballTiles);
    set_game_background(game1Map, game1Tiles, 16);

    set_active_basic_font(menuFont, BASIC_FONT_TILESET_SIZE);
    print_text("DEMO", 1, 3);
    print_text("A=CHARGE  B=RESET", 1, 15);
    print_text("START=TITLE", 1, 16);

    demo_score = 0;
    demo_score_prev = 0xFFFF;
    demo_charge = 0;
    demo_charge_prev = 0xFF;

    init_fill_bar();
    demo_draw_ui();

    // Spawn player ball
    demo_player = spawn_ball(24, 20);
    if (demo_player) {
        demo_player->physics.vx = 0;
        demo_player->physics.vy = 0;
    }

    // Turn on our screen, sprites, and BG
    platform_show_background();
    platform_show_sprites();
    platform_display_on();
}

void update_demo_scene(void)
{
    if (get_key_pressed(J_START))
        set_scene(SCENE_TITLE_SCREEN);

    if (demo_player) {
        if (get_key_pressed(J_LEFT))
            apply_impulse(demo_player, -FIXED_EIGHTH, 0);

        if (get_key_pressed(J_RIGHT))
            apply_impulse(demo_player, FIXED_EIGHTH, 0);

        if (get_key_held(J_A)) {
            if (demo_charge < 240)
                demo_charge++;
        } else if (get_key_released(J_A)) {
            fixed_t extra = TO_FIXED((demo_charge >> 4)); // 0..15
            launch_ball(demo_player, demo_player->transform.x, demo_player->transform.y, 0, -(LAUNCH_FORCE_Y + extra));
            demo_charge = 0;
#if MODULE_SOUND_ENABLED
            sound_play_square1(600, SOUND_DUTY_50, 12, SOUND_ENV_DOWN, 2, 16);
#endif
        }

        if (get_key_pressed(J_B)) {
            reset_all_balls();
            demo_score = 0;
        }
    }

    if (demo_charge != demo_charge_prev) {
        update_fill_bar(demo_charge);
        demo_charge_prev = demo_charge;
    }

    ball_update_all();

    // Events: collision -> score + sfx
    Event evt;
    while (events_poll(&evt)) {
        if (evt.type == EVENT_BALL_PIN_COLLISION) {
            if (demo_score < 99999u)
                demo_score++;
#if MODULE_SOUND_ENABLED
            sound_play_noise(8, SOUND_ENV_DOWN, 2, SOUND_NOISE_POLY(3, 5, 1), 8);
#endif
        }
    }

    if (demo_score != demo_score_prev) {
        demo_draw_ui();
        demo_score_prev = demo_score;
    }
}

void cleanup_demo_scene(void)
{
    platform_hide_sprites();
    platform_hide_background();
}

/* End of scenes/scene_demo.c */
