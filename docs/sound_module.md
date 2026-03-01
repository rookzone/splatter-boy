# Sound Module Guide

This module exposes basic Game Boy audio features:

- Square wave (channels 1 & 2)
- Wave table (channel 3)
- Noise (channel 4)

## Setup

Sound is enabled by default in `src/modules/modules_enabled.h`:

```
#define MODULE_SOUND_ENABLED    1
```

`modules_init()` automatically calls `module_sound_init()` which resets the sound system.

## Using sound in a scene

Include the module header in your scene file:

```
#include "../modules/sound.h"
```

### Example: simple square beep on button press

```
void update_game_scene(void)
{
    if (get_key_pressed(J_A)) {
        // freq_hz, duty, volume, env_dir, env_steps, length
        sound_play_square2(880, SOUND_DUTY_50, 12, SOUND_ENV_DOWN, 2, 20);
    }
}
```

### Example: load and play a wave (channel 3)

Wave channel uses a 32-sample waveform (4-bit samples packed into 16 bytes).
You can load it directly, or load from 32 bytes of 8-bit PCM samples.

```
static const uint8_t SINE_PCM_32[32] = {
    128,160,191,218,239,250,255,250,
    239,218,191,160,128,96, 64, 37,
    16, 5,  0,  5,  16, 37, 64, 96,
    128,160,191,218,239,250,255,250
};

void init_game_scene(void)
{
    sound_load_wave_pcm8(SINE_PCM_32);
}

void update_game_scene(void)
{
    if (get_key_pressed(J_B)) {
        sound_play_wave(440, SOUND_WAVE_LEVEL_100, 0);
    }
}
```

### Example: short noise burst (channel 4)

```
void update_game_scene(void)
{
    if (get_key_pressed(J_START)) {
        uint8_t poly = SOUND_NOISE_POLY(3, 6, 1);
        sound_play_noise(10, SOUND_ENV_DOWN, 3, poly, 20);
    }
}
```

## Notes

- If `length` is 0, the channel will sustain (length off).
- `sound_set_master_volume(left, right)` and `sound_set_panning(mask)` can be used for global tuning.
- For wave sounds from a `.wav` file, export **8-bit mono PCM**, then sample 32 values
  and pass them to `sound_load_wave_pcm8()`.
