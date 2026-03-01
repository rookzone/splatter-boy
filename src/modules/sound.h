// sound.h

#ifndef SOUND_H_
#define SOUND_H_

#include <stdint.h>

// Module: Sound
// Dependencies: core (platform/gb hardware)

// Duty cycles for square waves (channels 1 & 2)
#define SOUND_DUTY_12_5 0x00
#define SOUND_DUTY_25   0x40
#define SOUND_DUTY_50   0x80
#define SOUND_DUTY_75   0xC0

// Envelope direction
#define SOUND_ENV_DOWN 0
#define SOUND_ENV_UP   1

// Sweep direction (channel 1)
#define SOUND_SWEEP_UP   0
#define SOUND_SWEEP_DOWN 1

// Wave channel output levels (channel 3)
#define SOUND_WAVE_LEVEL_MUTE 0x00
#define SOUND_WAVE_LEVEL_100  0x20
#define SOUND_WAVE_LEVEL_50   0x40
#define SOUND_WAVE_LEVEL_25   0x60

// Panning bitmasks (for sound_set_panning)
#define SOUND_PAN_L_CH4 0x80
#define SOUND_PAN_L_CH3 0x40
#define SOUND_PAN_L_CH2 0x20
#define SOUND_PAN_L_CH1 0x10
#define SOUND_PAN_R_CH4 0x08
#define SOUND_PAN_R_CH3 0x04
#define SOUND_PAN_R_CH2 0x02
#define SOUND_PAN_R_CH1 0x01
#define SOUND_PAN_L_ALL (SOUND_PAN_L_CH1 | SOUND_PAN_L_CH2 | SOUND_PAN_L_CH3 | SOUND_PAN_L_CH4)
#define SOUND_PAN_R_ALL (SOUND_PAN_R_CH1 | SOUND_PAN_R_CH2 | SOUND_PAN_R_CH3 | SOUND_PAN_R_CH4)

// Noise polynomial helper
// div: 0-7, shift: 0-15, width7: 1 for 7-bit, 0 for 15-bit
#define SOUND_NOISE_POLY(div, shift, width7) (uint8_t)((((shift) & 0x0F) << 4) | ((width7) ? 0x08 : 0x00) | ((div) & 0x07))

// === CORE API ===

void sound_reset(void);
void sound_enable(uint8_t enabled);
void sound_set_master_volume(uint8_t left, uint8_t right);
void sound_set_panning(uint8_t pan_mask);

// Load wave data for channel 3
void sound_load_wave(const uint8_t* wave_16bytes);
void sound_load_wave_pcm8(const uint8_t* samples_32bytes);

// Play helpers (length = 0 means "length off")
void sound_set_square1_sweep(uint8_t time, uint8_t direction, uint8_t shift);
void sound_play_square1(uint16_t freq_hz, uint8_t duty, uint8_t volume, uint8_t env_dir, uint8_t env_steps, uint8_t length);
void sound_play_square2(uint16_t freq_hz, uint8_t duty, uint8_t volume, uint8_t env_dir, uint8_t env_steps, uint8_t length);
void sound_play_wave(uint16_t freq_hz, uint8_t level, uint8_t length);
void sound_play_noise(uint8_t volume, uint8_t env_dir, uint8_t env_steps, uint8_t poly, uint8_t length);

void sound_stop_channel(uint8_t channel); // 1-4
void sound_stop_all(void);

// === MODULE WRAPPER ===
void module_sound_init(void);
void module_sound_update(void);
void module_sound_shutdown(void);

#endif // SOUND_H_
