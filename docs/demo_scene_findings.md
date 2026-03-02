# Demo Scene Findings

This file records what surfaced while building a demo scene that uses common gameplay features.

## Demo scene coverage

- Input (move/charge)
- Graphics (background, sprites, text, UI bar)
- Physics (tile collision)
- Events (collision -> gameplay reaction)
- Sound (SFX on launch/collision)
- Object registry (ball update loop)

## Blockers / Generalization Opportunities

1) **Physics is ball-specific**
   - `check_ball_pin_collision()` and several constants assume a ball vs. pin tile.
   - Recommend: generalize into a tile-collision API that takes shape/size, or create a `physics_body` module and move ball-specific rules out.

2) **Events are ball-specific**
   - `EVENT_BALL_PIN_COLLISION` works but couples physics to ball/gameplay naming.
   - Recommend: use `EVENT_TILE_COLLISION` with tile type + object pointer, or `EVENT_COLLISION` with payload.

3) **Ball module provides gameplay, not engine**
   - Functions like `spawn_ball()`, `launch_ball()`, `find_lowest_ball()` are good for a sample but not generic.
   - Recommend: move common “entity” behaviors into a generic module, and keep `ball` as a thin gameplay layer.

4) **UI number printing**
   - Demo required a local `score_to_str` helper.
   - Recommend: add a lightweight number-print helper in graphics (e.g., `print_uint16()`).

## Core/Module separation checks

- `game_state.c` no longer includes module headers.
- Modules only depend on core.
- Scenes depend on modules and core.
