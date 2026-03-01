# Modules and Scenes Structure

This project separates **core**, **modules**, and **scenes**:

- **Core**: engine runtime model and shared state (types, game_state, world, platform).
- **Modules**: pure functionality (input, graphics, physics, gameplay modules like ball).
- **Scenes**: levels and game flow; they decide which modules to use and when.

## Directory layout

```
src/
  modules/
    input.c/.h
    graphics.c/.h
    physics.c/.h
    ball.c/.h
    sound.c/.h
    modules_registry.c/.h
    modules_enabled.h
  scenes/
    scene_*.c/.h
  world.c/.h
  grid.h
```

## Module dependencies (current)

- **input** → core only
- **graphics** → core only
- **physics** → core only (reads collision map from `world`)
- **ball** → graphics + physics + game_object (core)
- **sound** → core only (GB hardware)

Dependencies are declared in `src/modules/modules_registry.c` and noted in each module header.

## How dependency checks work

- Toggle modules in `src/modules/modules_enabled.h`.
- On startup, `modules_init()` checks for missing dependencies.
- If a dependency is missing, the engine shows a **module error screen** (uses graphics if enabled) and halts.

## How to add a module

1) Create `src/modules/<name>.c` and `src/modules/<name>.h`.
2) Expose wrapper API:
   - `module_<name>_init()`
   - `module_<name>_update()` (thin, or no-op)
   - `module_<name>_shutdown()`
3) Add a small dependency comment at the top of the module header.
4) Register it in `src/modules/modules_registry.c`:
   - include the header (guarded by `MODULE_<NAME>_ENABLED`)
   - add a `MODULE_BIT_<NAME>` and a table entry with dependencies
   - call its init/update/shutdown in the registry functions
5) Add an enable toggle in `src/modules/modules_enabled.h`.
6) Use it from scenes by including the header and calling its functions.

## How to build a scene

1) Create `src/scenes/scene_<name>.c` and `src/scenes/scene_<name>.h`.
2) Add it to `src/scene_manager.h` and `src/scene_manager.c`.
3) In `init_<name>_scene()`:
   - load background and sprites
   - spawn objects
   - optionally set collision map via `world_set_collision_tilemap()`
4) In `update_<name>_scene()`:
   - read input
   - call module functions needed for that scene (e.g., `ball_update_all()`)
5) In `cleanup_<name>_scene()`:
   - hide sprites/background and release any transient state

## Collision map note (physics)

Physics reads collision tiles from `world_get_collision_tilemap()`.  
`set_game_background()` now sets the collision map by default, but scenes can override it:

```
world_set_collision_tilemap(my_collision_map);
```

This keeps **physics independent from graphics** while still allowing scenes to define collision behavior.
