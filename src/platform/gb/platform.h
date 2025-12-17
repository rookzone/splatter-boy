#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <gb/gb.h>
#include <gb/drawing.h>
#include <gbdk/console.h>
#include <stdint.h>

/**
 * PLATFORM WRAPPERS (GBDK SPECIFIC)
 * These functions wrap GBDK calls to decouple game logic from the SDK.
 * Using static inline ensures zero overhead.
 */

// --- System & Hardware ---

static inline void platform_display_off(void) {
    DISPLAY_OFF;
}

static inline void platform_display_on(void) {
    DISPLAY_ON;
}

static inline void platform_sprites_8x8(void) {
    SPRITES_8x8;
}

static inline void platform_show_sprites(void) {
    SHOW_SPRITES;
}

static inline void platform_hide_sprites(void) {
    HIDE_SPRITES;
}

static inline void platform_show_background(void) {
    SHOW_BKG;
}

static inline void platform_hide_background(void) {
    HIDE_BKG;
}

static inline uint8_t platform_get_input(void) {
    return joypad();
}

static inline void platform_wait_vbl(void) {
    wait_vbl_done();
}

static inline void platform_vsync(void) {
    vsync();
}

// --- Sprite Operations ---

static inline void platform_set_sprite_tile(uint8_t nb, uint8_t tile) {
    set_sprite_tile(nb, tile);
}

static inline void platform_set_sprite_data(uint8_t first, uint8_t nb, unsigned char *data) {
    set_sprite_data(first, nb, data);
}

static inline void platform_move_sprite(uint8_t nb, uint8_t x, uint8_t y) {
    move_sprite(nb, x, y);
}

// --- Background & VRAM Operations ---

static inline void platform_set_bkg_data(uint8_t first, uint8_t nb, unsigned char *data) {
    set_bkg_data(first, nb, data);
}

static inline void platform_set_bkg_tiles(uint8_t x, uint8_t y, uint8_t w, uint8_t h, unsigned char *tiles) {
    set_bkg_tiles(x, y, w, h, tiles);
}

static inline void platform_set_vram_byte(uint8_t *addr, uint8_t byte) {
    set_vram_byte(addr, byte);
}

static inline uint8_t* platform_get_bkg_xy_addr(uint8_t x, uint8_t y) {
    return get_bkg_xy_addr(x, y);
}

// --- Drawing & Console ---

static inline void platform_plot_point(uint8_t x, uint8_t y) {
    plot_point(x, y);
}

static inline void platform_console_gotoxy(uint8_t x, uint8_t y) {
    gotoxy(x, y);
}

#endif // PLATFORM_H_