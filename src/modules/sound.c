// sound.c

#include "sound.h"
#include <gb/hardware.h>
#include <stddef.h>

#define SOUND_VOL_LEFT(x)  ((uint8_t)((x) << 4))
#define SOUND_VOL_RIGHT(x) ((uint8_t)((x) & 0x07))

static uint16_t sound_freq_to_reg(uint16_t freq_hz)
{
    if (freq_hz == 0) {
        return 0;
    }

    // x = 2048 - (131072 / freq_hz)
    uint32_t div = 131072u / freq_hz;
    if (div > 2048u) {
        div = 2048u;
    }
    return (uint16_t)(2048u - div);
}

static uint8_t sound_length_bit(uint8_t length)
{
    return (length == 0) ? AUDHIGH_LENGTH_OFF : AUDHIGH_LENGTH_ON;
}

void sound_reset(void)
{
    rAUDENA = AUDENA_ON;
    sound_set_master_volume(7, 7);
    sound_set_panning(SOUND_PAN_L_ALL | SOUND_PAN_R_ALL);
}

void sound_enable(uint8_t enabled)
{
    rAUDENA = enabled ? AUDENA_ON : AUDENA_OFF;
}

void sound_set_master_volume(uint8_t left, uint8_t right)
{
    if (left > 7) left = 7;
    if (right > 7) right = 7;
    rAUDVOL = (uint8_t)(SOUND_VOL_LEFT(left) | SOUND_VOL_RIGHT(right));
}

void sound_set_panning(uint8_t pan_mask)
{
    rAUDTERM = pan_mask;
}

void sound_load_wave(const uint8_t* wave_16bytes)
{
    if (wave_16bytes == NULL)
        return;

    // Channel 3 must be disabled when writing wave RAM
    uint8_t was_enabled = rAUD3ENA;
    rAUD3ENA = 0;
    for (uint8_t i = 0; i < 16; i++) {
        AUD3WAVE[i] = wave_16bytes[i];
    }
    rAUD3ENA = was_enabled;
}

void sound_load_wave_pcm8(const uint8_t* samples_32bytes)
{
    if (samples_32bytes == NULL)
        return;

    uint8_t was_enabled = rAUD3ENA;
    rAUD3ENA = 0;
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t hi = (uint8_t)(samples_32bytes[i * 2] >> 4);
        uint8_t lo = (uint8_t)(samples_32bytes[i * 2 + 1] >> 4);
        AUD3WAVE[i] = (uint8_t)((hi << 4) | lo);
    }
    rAUD3ENA = was_enabled;
}

void sound_set_square1_sweep(uint8_t time, uint8_t direction, uint8_t shift)
{
    rAUD1SWEEP = (uint8_t)(((time & 0x07) << 4) |
                           ((direction ? 1u : 0u) << 3) |
                           (shift & 0x07));
}

void sound_play_square1(uint16_t freq_hz, uint8_t duty, uint8_t volume, uint8_t env_dir, uint8_t env_steps, uint8_t length)
{
    uint16_t reg = sound_freq_to_reg(freq_hz);

    rAUD1LEN = (uint8_t)((duty & 0xC0) | (length & 0x3F));
    rAUD1ENV = (uint8_t)(((volume & 0x0F) << 4) |
                         ((env_dir ? 1u : 0u) << 3) |
                         (env_steps & 0x07));
    rAUD1LOW = (uint8_t)(reg & 0xFF);
    rAUD1HIGH = (uint8_t)(((reg >> 8) & 0x07) | sound_length_bit(length) | AUDHIGH_RESTART);
}

void sound_play_square2(uint16_t freq_hz, uint8_t duty, uint8_t volume, uint8_t env_dir, uint8_t env_steps, uint8_t length)
{
    uint16_t reg = sound_freq_to_reg(freq_hz);

    rAUD2LEN = (uint8_t)((duty & 0xC0) | (length & 0x3F));
    rAUD2ENV = (uint8_t)(((volume & 0x0F) << 4) |
                         ((env_dir ? 1u : 0u) << 3) |
                         (env_steps & 0x07));
    rAUD2LOW = (uint8_t)(reg & 0xFF);
    rAUD2HIGH = (uint8_t)(((reg >> 8) & 0x07) | sound_length_bit(length) | AUDHIGH_RESTART);
}

void sound_play_wave(uint16_t freq_hz, uint8_t level, uint8_t length)
{
    uint16_t reg = sound_freq_to_reg(freq_hz);

    rAUD3ENA = 0x80;
    rAUD3LEN = length;
    rAUD3LEVEL = (uint8_t)(level & 0x60);
    rAUD3LOW = (uint8_t)(reg & 0xFF);
    rAUD3HIGH = (uint8_t)(((reg >> 8) & 0x07) | sound_length_bit(length) | AUDHIGH_RESTART);
}

void sound_play_noise(uint8_t volume, uint8_t env_dir, uint8_t env_steps, uint8_t poly, uint8_t length)
{
    rAUD4LEN = (uint8_t)(length & 0x3F);
    rAUD4ENV = (uint8_t)(((volume & 0x0F) << 4) |
                         ((env_dir ? 1u : 0u) << 3) |
                         (env_steps & 0x07));
    rAUD4POLY = poly;
    rAUD4GO = (uint8_t)(sound_length_bit(length) | AUDHIGH_RESTART);
}

void sound_stop_channel(uint8_t channel)
{
    switch (channel) {
        case 1:
            rAUD1ENV = 0;
            rAUD1HIGH = 0;
            break;
        case 2:
            rAUD2ENV = 0;
            rAUD2HIGH = 0;
            break;
        case 3:
            rAUD3ENA = 0;
            break;
        case 4:
            rAUD4ENV = 0;
            rAUD4GO = 0;
            break;
        default:
            break;
    }
}

void sound_stop_all(void)
{
    sound_stop_channel(1);
    sound_stop_channel(2);
    sound_stop_channel(3);
    sound_stop_channel(4);
}

// === MODULE WRAPPER ===

void module_sound_init(void)
{
    sound_reset();
}

void module_sound_update(void)
{
    // No-op for now (kept for module parity)
}

void module_sound_shutdown(void)
{
    sound_enable(0);
}

/* End of sound.c */
