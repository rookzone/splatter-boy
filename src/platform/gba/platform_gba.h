// platform/gb/platform_gb.h

#ifndef PLATFORM_GB_H_
#define PLATFORM_GB_H_

#include <stdint.h>


static inline void platform_display_off(void) {

}

static inline void platform_display_on(void) {
 
}

static inline void platform_sprites_8x8(void) {

}

static inline void platform_show_sprites(void) {

}

static inline void platform_hide_sprites(void) {

}

static inline void platform_show_background(void) {

}

static inline void platform_hide_background(void) {

}

static inline uint8_t platform_get_input(void) {

}

static inline void platform_wait_vbl(void) {

}

static inline void platform_vsync(void) {

}

// === Sprite Operations ===

static inline void platform_set_sprite_tile(uint8_t nb, uint8_t tile) {

}

static inline void platform_set_sprite_data(uint8_t first, uint8_t nb, unsigned char *data) {

}

static inline void platform_move_sprite(uint8_t nb, uint8_t x, uint8_t y) {

}

// === Background & VRAM Operations ===

static inline void platform_set_bkg_data(uint8_t first, uint8_t nb, unsigned char *data) {

}

static inline void platform_set_bkg_tiles(uint8_t x, uint8_t y, uint8_t w, uint8_t h, unsigned char *tiles) {

}

static inline void platform_set_vram_byte(uint8_t *addr, uint8_t byte) {

}

static inline uint8_t* platform_get_bkg_xy_addr(uint8_t x, uint8_t y) {

}

// === Drawing & Console ===

static inline void platform_plot_point(uint8_t x, uint8_t y) {

}

static inline void platform_console_gotoxy(uint8_t x, uint8_t y) {

}



#endif // SCENE_GAME_H_