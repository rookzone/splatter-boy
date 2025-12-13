Here are the rapid development workflows tailored to your new Data-Oriented architecture. Keep these as a checklist to ensure you don't miss a connection when adding new features.

### 1. Workflow: New GameObject
*Example: Adding a "Spinner" or "Flipper"*

1.  **Define Type:**
    * In `custom_types.h`: Add `OBJ_SPINNER` to the `ObjectType` enum.
2.  **Update Data Storage:**
    * In `game_data.h`: Add a registry array to `ObjectManager`.
        * `uint8_t spinner_indices[MAX_SPINNERS];`
        * `uint8_t spinner_count;`
3.  **Create Logic Module:**
    * Create `spinner.c` and `spinner.h`.
    * **Spawn Function:** Create `spawn_spinner(x, y)`.
        * Call `go_spawn_object(OBJ_SPINNER)`.
        * Set Flags: `obj->flags |= (TRANSFORM_ACTIVE | RENDERER_ACTIVE);`
        * Initialize components (`obj->transform.x = x`, etc.).
    * **Update Function:** Create `update_spinner(GameObject* obj)`.
4.  **Register Logic:**
    * In `game_object.c` -> `go_spawn_object()`: Add the `case OBJ_SPINNER` to register the new object into its specific `spinner_indices` array.
5.  **Create Loop:**
    * In `game_object.c`: Create `go_update_all_spinners()`.
    * *Optimisation:* Iterate through `game.objects.spinner_indices` (not the whole pool).
    * Call `update_spinner(obj)` inside the loop.
6.  **Hook into State:**
    * In `state_game_screen.c`: Call `go_update_all_spinners()` inside `update_game_screen()`.

---

### 2. Workflow: New Component
*Example: Adding "Health" or "Animation"*

1.  **Define Data Structure:**
    * In `custom_types.h`: Define `typedef struct { uint8_t hp; } HealthComponent;`.
2.  **Define Flag:**
    * In `custom_types.h`: Add `#define HEALTH_ACTIVE 0x10` (Ensure it is the next power of 2: 1, 2, 4, 8, **16**, 32...).
3.  **Add to GameObject:**
    * In `game_object.h`: Add `HealthComponent health;` to the `GameObject` struct.
4.  **Update Spawners:**
    * In your object spawn functions (e.g., `spawn_enemy`):
        * Add the flag: `obj->flags |= HEALTH_ACTIVE;`.
        * Initialize data: `obj->health.hp = 100;`.

---

### 3. Workflow: New System (Global Logic)
*Example: Adding an Input System or Audio System*

1.  **Create Module:**
    * Create `sys_input.c` and `sys_input.h`.
2.  **Define Update:**
    * Create `void sys_input_update(void)`.
3.  **Access Data:**
    * Include `game_data.h`.
    * Read/Write directly to `game.system` or `game.objects`.
    * *Example:* `if (joypad() & J_A) { ...launch logic... }`
4.  **Hook into State:**
    * In `state_game_screen.c`: Call `sys_input_update()` at the start of `update_game_screen()`.

---

### 4. Workflow: New Game State
*Example: Adding a Score Screen or Main Menu*

1.  **Define ID:**
    * In `state_manager.h`: Add `#define STATE_SCORE_SCREEN 3`.
2.  **Create Module:**
    * Create `states/state_score.c` and `states/state_score.h`.
3.  **Implement Triad:**
    * `void init_score(void)`: Load tiles, set background, reset specific variables.
    * `void update_score(void)`: Check for "Start" button to leave state.
    * `void cleanup_score(void)`: Hide sprites, turn off display (if needed for VRAM load).
4.  **Register:**
    * In `state_manager.c`: Add `case STATE_SCORE_SCREEN` to:
        * `init_state()`
        * `update_state()`
        * `cleanup_state()`

---

### 5. Workflow: New Asset (Graphics)
*Example: Adding a new sprite sheet*

1.  **Generate:**
    * Use GBTD/GBMB to export `.c` and `.h` files.
2.  **Import:**
    * Place in `/tiles` or `/maps`.
    * Include the `.h` file in `state_game_screen.c` (or wherever it is loaded).
3.  **Load:**
    * In `init_game_screen()`:
        * `set_sprite_sheet(NewTiles);`
    * *Note:* Remember GBDK sprite limits (max 128 tiles in VRAM for sprites).
