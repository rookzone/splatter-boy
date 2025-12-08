# Game Architecture Refactor Plan

## **Core Philosophy**

**All game state lives in `game_data.h/c`** - This becomes your single source of truth. States initialize and manipulate this data through clean APIs. No hidden state scattered across modules.

---

## **New Game Data Structure**

```c
// game_data.h

typedef struct {
    // === SYSTEM ===
    uint8_t current_state;
    uint8_t keys;
    uint8_t previous_keys;
} System;

typedef struct {
    // Sprite allocation tracking
    uint8_t next_sprite_id;          // Next available hardware sprite slot (0-39)
    uint8_t sprite_count;            // Total sprites currently in use
    
    // Active assets (pointers to ROM data)
    const unsigned char* active_background_tilemap;
    const unsigned char* active_background_tiledata;
    const unsigned char* active_sprite_sheet;
} Graphics;

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;             // Total active objects
    
    // Registries (indices into pool for fast iteration by type)
    uint8_t ball_indices[MAX_BALLS];
    uint8_t ball_count;
    
    uint8_t pin_indices[MAX_PINS];
    uint8_t pin_count;
    
    // Could add more registries:
    // uint8_t wall_indices[MAX_WALLS];
    // uint8_t wall_count;
} ObjectManager;

typedef struct {
    System system;
    Graphics graphics;
    ObjectManager objects;
    
    // Future: could add more subsystems
    // Audio audio;
    // Score score;
} GameState;

extern GameState game;
```

**Why this works:**
- **Single allocation** - All game memory in one struct
- **Clear ownership** - `game.objects.pool` owns all objects
- **Fast queries** - Registries let you iterate only balls, only pins, etc.
- **Easy to save/load** - All state in one place (for future save system)

---

## **Data Flow Architecture**

```
┌─────────────────────────────────────────────────────────┐
│                      GAME DATA                          │
│  (game_data.h/c - Single Source of Truth)              │
│                                                         │
│  ┌───────────────────────────────────────────────┐    │
│  │ ObjectManager                                  │    │
│  │   - pool[32]         (all GameObjects)        │    │
│  │   - ball_indices[]   (which pool slots = balls)│   │
│  │   - pin_indices[]    (which pool slots = pins) │   │
│  └───────────────────────────────────────────────┘    │
│                                                         │
│  ┌───────────────────────────────────────────────┐    │
│  │ Graphics                                       │    │
│  │   - next_sprite_id   (VRAM sprite allocator)  │    │
│  │   - sprite_count     (usage tracking)         │    │
│  └───────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
                          ▲
                          │ reads/writes
                          │
┌─────────────────────────────────────────────────────────┐
│                    API MODULES                          │
│  (Manipulate game data, don't store state)             │
│                                                         │
│  game_object.c  - Spawn/update/draw objects            │
│  physics.c      - Apply forces, collision              │
│  graphics.c     - Sprite allocation, drawing           │
│  ball.c         - Ball-specific logic                  │
│  pin.c          - Pin-specific logic                   │
└─────────────────────────────────────────────────────────┘
                          ▲
                          │ calls APIs
                          │
┌─────────────────────────────────────────────────────────┐
│                    STATE LOGIC                          │
│  (Configure game data for each state)                  │
│                                                         │
│  state_game_screen.c  - Spawns balls/pins, runs game   │
│  state_title_screen.c - Shows menu, waits for input    │
│  state_score_screen.c - Displays results               │
└─────────────────────────────────────────────────────────┘
```

---

## **Refactor Steps (In Order)**

### **PHASE 1: Flatten GameObject Components**

**Goal:** Remove union, add flat components to GameObject

#### 1.1 Update `custom_types.h`
- [ ] Create `PositionComponent` struct (x, y)
- [ ] Create `PhysicsComponent` struct (sub_x, sub_y, vx, vy)
- [ ] Create `RenderComponent` struct (sprite_index, tile_index)
- [ ] Remove old `Ball`, `Pin`, `Wall` typedefs (they're obsolete)
- [ ] Keep `ObjectType` enum (OBJ_BALL, OBJ_PIN, OBJ_WALL)

#### 1.2 Update `game_object.h`
- [ ] Redefine `GameObject` struct with flat components:
  ```c
  struct GameObject {
      ObjectType object_type;
      uint8_t active;
      UpdateFunc update;
      PositionComponent pos;
      PhysicsComponent physics;
      RenderComponent render;
  };
  ```
- [ ] Remove the `union` and `data` member entirely
- [ ] Move `ObjectIndices` struct to `game_data.h` (rename to part of `ObjectManager`)

#### 1.3 Update `game_object.c`
- [ ] Remove static `game_object_pool` - it will move to `game_data.c`
- [ ] Remove static `indices` - it will move to `game_data.c`
- [ ] Update all functions to reference `game.objects.pool` instead of local pool
- [ ] Update all functions to reference `game.objects.ball_indices` etc.

---

### **PHASE 2: Centralize Data in game_data**

**Goal:** Move all state into the `GameState` struct

#### 2.1 Update `game_data.h`
- [ ] Add `ObjectManager` struct (contains pool and registries)
- [ ] Update `Graphics` struct to include `sprite_count`
- [ ] Update `GameState` to include `ObjectManager objects`

#### 2.2 Update `game_data.c`
- [ ] Initialize `GameState game` (already exists, just update the struct)

#### 2.3 Update `game_object.c`
- [ ] Change `go_init_manager()` to reset `game.objects.*` instead of static vars
- [ ] Change `go_spawn_object()` to allocate from `game.objects.pool`
- [ ] Change `go_update_all()` to iterate through `game.objects.ball_indices`
- [ ] Change `go_draw_all()` to iterate through `game.objects.ball_indices` + `pin_indices`
- [ ] Change `go_return_ball()` to index into `game.objects.pool`

---

### **PHASE 3: Update Graphics System**

**Goal:** Sprite allocation managed through game_data

#### 3.1 Update `graphics.h`
- [ ] Update `DRAW_SPRITE` macro to work with `RenderComponent*`
  ```c
  #define DRAW_SPRITE(render_ptr, x, y) \
      move_sprite((render_ptr)->sprite_index, (x) + 8, (y) + 16)
  ```

#### 3.2 Update `graphics.c`
- [ ] Update `create_sprite()` to use `game.graphics.next_sprite_id`
- [ ] Update `create_sprite()` to increment `game.graphics.sprite_count`
- [ ] Add `release_sprite()` function to decrement sprite_count (for future use)
- [ ] Add safety check: if `next_sprite_id >= 40`, handle error (return null sprite or reuse)

---

### **PHASE 4: Update Object-Specific Modules**

**Goal:** Use flat components instead of union access

#### 4.1 Update `ball.h`
- [ ] Remove `Ball` typedef (no longer needed - it's just components)
- [ ] Change `spawn_ball()` signature to return `GameObject*`
- [ ] Remove `init_ball()` - logic moves into `spawn_ball()`
- [ ] Update function signatures to take `GameObject*` instead of `Ball*`

#### 4.2 Update `ball.c`
- [ ] Update `spawn_ball()` to initialize flat components:
  ```c
  obj->pos.x = x;
  obj->pos.y = y;
  obj->physics.vx = 0;
  obj->render = create_sprite(TILE_BALL);
  ```
- [ ] Update `update_ball()` to use `obj->pos.x` instead of `obj->data.ball.x`
- [ ] Update `reset_all_balls()` to access `obj->pos` and `obj->physics` directly
- [ ] Remove all `.data.ball` accesses

#### 4.3 Update `pin.h`
- [ ] Remove `Pin` typedef (no longer needed)
- [ ] Add `spawn_pin()` function (similar to spawn_ball)
- [ ] Remove `init_pin()` and `init_background_pin()`

#### 4.4 Update `pin.c`
- [ ] Create `spawn_pin()` function:
  ```c
  GameObject* spawn_pin(uint8_t x, uint8_t y) {
      GameObject* obj = go_spawn_object(OBJ_PIN);
      obj->pos.x = x;
      obj->pos.y = y;
      obj->render = create_sprite(TILE_PIN);
      // physics component ignored for pins
      return obj;
  }
  ```

#### 4.5 Update `physics.h`
- [ ] Change all function signatures from `Ball*` to `GameObject*`
- [ ] Remove `Ball*` and `Pin*` parameters, use `GameObject*` everywhere

#### 4.6 Update `physics.c`
- [ ] Update `update_ball_position()` to use `obj->physics` and `obj->pos`
- [ ] Update `handle_ball_pin_collision()` to take two `GameObject*` (or one GameObject* and pos x,y)
- [ ] Update all collision functions to use flat component access
- [ ] Remove all `.data.ball` accesses

---

### **PHASE 5: Update State Logic**

**Goal:** States initialize game_data and use APIs to manipulate it

#### 5.1 Update `state_game_screen.c`
- [ ] In `init_game_screen()`:
  - Call `go_init_manager()` (resets `game.objects`)
  - Reset `game.graphics.next_sprite_id = 0`
  - Reset `game.graphics.sprite_count = 0`
  - Call `spawn_ball()` for each ball (stores in `game.objects.pool`)
  - If spawning pins as sprites, call `spawn_pin()`
- [ ] In `update_game_screen()`:
  - No changes needed (just calls `go_update_all()` and `go_draw_all()`)
- [ ] In `cleanup_game_screen()`:
  - Could add sprite cleanup if needed

#### 5.2 Update `state_title_screen.c` (when you create it)
- [ ] Initialize title screen graphics
- [ ] Don't spawn game objects (menu only)
- [ ] Handle input to transition to game state

---

### **PHASE 6: Clean Up & Test**

#### 6.1 Remove Dead Code
- [ ] Remove unused `Ball`, `Pin`, `Wall` typedefs from custom_types.h (unless used for collision helpers)
- [ ] Remove any remaining union references
- [ ] Remove commented-out old code

#### 6.2 Add Helper Macros (Optional)
- [ ] Add component existence checks:
  ```c
  #define HAS_PHYSICS(obj) ((obj)->object_type == OBJ_BALL)
  #define HAS_RENDER(obj)  ((obj)->object_type != OBJ_WALL)
  ```

#### 6.3 Test Each System
- [ ] Test ball spawning and movement
- [ ] Test collision detection with pins
- [ ] Test sprite allocation (ensure no overflow past 40 sprites)
- [ ] Test state transitions
- [ ] Test reset_all_balls()

---

## **Key Design Principles**

### **1. Data Ownership**
```
game_data.c owns ALL mutable state
↓
API modules (game_object.c, physics.c) manipulate it
↓
States (state_game_screen.c) orchestrate the APIs
```

### **2. No Hidden State**
- ❌ No static variables in .c files (except const lookup tables)
- ✅ Everything lives in `game` global
- ✅ Easy to inspect in debugger: just look at `game`

### **3. Clear Boundaries**
- **game_data**: Storage only (struct definitions)
- **APIs**: Logic only (functions that modify game_data)
- **States**: Orchestration only (call APIs in the right order)

### **4. Component Usage**
```c
// OBJ_BALL uses all components
obj->pos.x       // ✓ Used
obj->physics.vx  // ✓ Used
obj->render.sprite_index // ✓ Used

// OBJ_PIN uses pos + render
obj->pos.x       // ✓ Used
obj->physics.vx  // ✗ Ignored (but takes memory - acceptable tradeoff)
obj->render.sprite_index // ✓ Used

// OBJ_WALL uses pos only
obj->pos.x       // ✓ Used (for collision checks)
obj->physics.vx  // ✗ Ignored
obj->render.sprite_index // ✗ Ignored (drawn via background)
```

---

## **Quick Reference: Before & After**

### **Before (Current)**
```c
// Spawning
Ball* ball = &go_return_ball(i)->data.ball;

// Accessing
ball->vx = 100;
obj->x = ball->x;  // Duplication!

// Drawing
DRAW_SPRITE(&obj->sprite, obj->x, obj->y);
```

### **After (New)**
```c
// Spawning
GameObject* obj = spawn_ball(10, 20);

// Accessing
obj->physics.vx = 100;
// No duplication - pos is the single source

// Drawing
DRAW_SPRITE(&obj->render, obj->pos.x, obj->pos.y);
```

---

## **Testing Checklist**

After each phase, verify:
- [ ] Game compiles without errors
- [ ] Balls spawn and fall with gravity
- [ ] Balls bounce off pins correctly
- [ ] Sprites render at correct positions
- [ ] No crashes or memory corruption
- [ ] State transitions work (if implemented)

---

Gemini AI drafted task list:

This is a solid architectural move. transitioning from a **tagged union** (Specific types) to a **Component-based** (Composition) architecture is standard for game development.

For the Game Boy (GBDK/SDCC), this is actually **faster** in many cases because you avoid the pointer redirection of `obj->data.ball.x`. If you keep the data "flat" (contiguous in memory), the CPU can access it very quickly.

Here is a phased plan to refactor your code into a "Unity-Lite" system.

-----

### Phase 1: The Data Structure (The "Skeleton")

**Goal:** Flatten `GameObject` and move the Manager into `game_data`.

Instead of `union { Ball b; Pin p; }`, every `GameObject` will have a `Transform`, `PhysicsBody`, and `Renderer` struct built directly inside it. If an object doesn't need physics (like a static decoration), we just ignore those fields using flags.

**Tasks:**

1.  **Define Components in `custom_types.h`:** Create simple structs for `Transform` (x, y), `Physics` (vx, vy, mass/elasticity), and `Renderer` (sprite id, tile id).
2.  **Redefine `GameObject`:** Remove the union. Include the components directly. Add a `flags` bitmask to determine which components are active (e.g., `HAS_PHYSICS`, `HAS_COLLIDER`).
3.  **Update `GameState`:** Move the `ObjectManager` struct definition inside the `GameState` struct in `game_data.h`.

**Snippet (`custom_types.h` / `game_object.h`):**

```c
// Bitmasks for component activity
#define COMP_ACTIVE     0x01
#define COMP_PHYSICS    0x02
#define COMP_RENDER     0x04
#define COMP_COLLIDER   0x08 

typedef struct {
    uint8_t x, y;
    fixed_n fractional_x, fractional_y; // Keep fixed point logic here
} Transform;

typedef struct {
    fixed_n vx, vy;
    uint8_t gravity_scale; // 0 for no gravity, 1 for normal
} PhysicsBody;

typedef struct {
    uint8_t sprite_index;
    uint8_t tile_index;
    uint8_t visible; 
} Renderer;

struct GameObject {
    uint8_t id;          // Useful for debugging
    uint8_t flags;       // Which components are on?
    uint8_t tag;         // OBJ_BALL, OBJ_PIN (still useful for logic)
    
    // Components are embedded directly (Data Locality)
    Transform transform;
    PhysicsBody physics;
    Renderer renderer;
    // Input input;      // Future scope
};
```

-----

### Phase 2: Centralizing the Manager (The "Brain")

**Goal:** Move ownership of the object pool to `game_data` so the State Machine owns the memory.

**Tasks:**

1.  **Refactor `game_data.h`:** Ensure `ObjectManager` contains the `GameObject pool[MAX_OBJECTS]` and the `count`.
2.  **Refactor `game_object.c`:**
      * Remove the `static GameObject game_object_pool` array.
      * Update `go_spawn_object` to look for a free slot in `game.manager.pool`.
      * Update `go_init_manager` to clear `game.manager`.
3.  **Update `main` / `state_game_screen`:** Ensure `game` struct is initialized zeroed out on startup.

**Snippet (`game_object.c`):**

```c
GameObject* go_spawn(uint8_t tag, uint8_t x, uint8_t y) {
    // Access the central store
    ObjectManager* mgr = &game.manager; 
    
    if (mgr->total_count >= MAX_GAME_OBJECTS) return NULL;

    // Find first inactive slot (simple optimization: keep a 'next_free' index)
    GameObject* obj = &mgr->pool[mgr->total_count]; 
    
    // Reset data
    memset(obj, 0, sizeof(GameObject));
    
    // Default Transform setup
    obj->flags |= COMP_ACTIVE;
    obj->tag = tag;
    obj->transform.x = x;
    obj->transform.y = y;
    
    mgr->total_count++;
    return obj;
}
```

-----

### Phase 3: The Systems (The "Muscles")

**Goal:** Rewrite Physics and Graphics to accept generic `GameObject` pointers and check flags, rather than expecting specific types like `Ball`.

**Tasks:**

1.  **Update `physics.c`:**
      * Change functions to accept `GameObject* obj`.
      * At the start of the function, check `if (!(obj->flags & COMP_PHYSICS)) return;`.
      * Update math to access `obj->transform.x` and `obj->physics.vx`.
2.  **Update `graphics.c`:**
      * Update `go_draw_all`. It should loop through the pool, check `(flags & COMP_RENDER)`, and draw using `obj->renderer` data and `obj->transform` position.
3.  **Remove `ball.c` / `pins.c` structs:** You might keep these files for *Constructors* (e.g., `make_ball(obj)`) or *Specific Logic* (e.g., `update_ball_logic(obj)`), but they no longer define the data structure.

**Snippet (New Physics Loop):**

```c
void physics_update_system(void) {
    ObjectManager* mgr = &game.manager;
    
    for(uint8_t i = 0; i < mgr->total_count; i++) {
        GameObject* obj = &mgr->pool[i];
        
        // Skip dead or static objects immediately
        if ((obj->flags & (COMP_ACTIVE | COMP_PHYSICS)) != (COMP_ACTIVE | COMP_PHYSICS)) 
            continue;

        // Apply Gravity
        obj->physics.vy += GRAVITY;
        
        // Move Transform
        // ... (Your existing fixed point math here, accessing obj->transform)
    }
}
```

-----

### Phase 4: The API & Gameplay (The "Interface")

**Goal:** The gameplay code (`state_game_screen.c`) should use high-level "Builders" or "Factories" to create entities.

**Tasks:**

1.  **Create Factory Functions:** Instead of `spawn_ball`, create a function that grabs a raw GameObject and configures its components.
2.  **Refactor `state_game_screen.c`:**
      * `init`: Call `go_init_manager()`.
      * `init`: Call `factory_create_ball(x,y)` for your setup loop.
      * `update`: Call `go_update_systems()` (which calls physics, then specific logic, then render).

**Snippet (Factory in `ball.c`):**

```c
GameObject* factory_create_ball(uint8_t x, uint8_t y) {
    GameObject* obj = go_spawn(OBJ_BALL, x, y);
    
    // Turn on components
    obj->flags |= (COMP_PHYSICS | COMP_RENDER | COMP_COLLIDER);
    
    // Setup Physics
    obj->physics.vx = 0;
    
    // Setup Graphics
    obj->renderer.tile_index = TILE_BALL;
    obj->renderer.sprite_index = alloc_sprite_id(); 
    
    return obj;
}
```

-----

### Performance Reminders for GBDK

1.  **Pointer Caching:** In your loops, don't do `game.manager.pool[i].transform.x` repeatedly.
      * *Do:* `GameObject* obj = &game.manager.pool[i];` then `obj->transform.x`.
2.  **Structure Padding:** GBDK (SDCC) is usually 8-bit. Try to order your struct members so `x` and `y` are next to each other, and commonly accessed flags are at the top.
3.  **The "Update" Function:**
      * Previously you had `obj->update = update_ball`.
      * In a Component system, you usually iterate **Systems** (Physics System, Render System) over all objects.
      * *However*, for specific logic (like "If Ball hits X, add Score"), you can still keep a function pointer `obj->behaviour` or use a switch statement on `obj->tag` inside a generic loop. A switch statement on `tag` is often faster and uses less RAM than function pointers for every object on the Game Boy.

### Next Step for User

Would you like me to generate the code for **Phase 1 (The Structs)** so you can copy-paste that into `custom_types.h` and `game_data.h` to get started?
