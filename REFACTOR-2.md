# Game Boy Component System Refactor Plan

## Overview

This refactor moves from a union-based GameObject system to a **flat component-based architecture** with centralized state management. All game state lives in `GameState game`, and objects are composed of components rather than type-specific structs.

---

## Core Architecture

```
┌─────────────────────────────────────────────────────┐
│                   GAME STATE                        │
│              (Single Source of Truth)               │
│                                                     │
│  ObjectManager                                      │
│    └─ pool[32]          (GameObject array)         │
│    └─ ball_indices[]    (registry for balls)       │
│    └─ ball_count                                    │
│                                                     │
│  Graphics                                           │
│    └─ next_sprite_id    (VRAM allocator)           │
│    └─ sprite_count                                  │
│    └─ active_background_tilemap                     │
│    └─ active_background_tiledata                    │
│    └─ active_sprite_sheet                           │
└─────────────────────────────────────────────────────┘
                         ▲
                         │
                         │ APIs manipulate
                         │
┌─────────────────────────────────────────────────────┐
│              API MODULES (Stateless)                │
│                                                     │
│  game_object.c  - Object lifecycle management      │
│  physics.c      - Physics system (forces, motion)  │
│  graphics.c     - Rendering & sprite allocation    │
│  ball.c         - Ball factory & behavior          │
│  pin.c          - Pin factory & behavior           │
└─────────────────────────────────────────────────────┘
                         ▲
                         │
                         │ States orchestrate
                         │
┌─────────────────────────────────────────────────────┐
│              GAME STATES                            │
│                                                     │
│  state_game_screen.c  - Main gameplay              │
│  state_title_screen.c - Menu/intro                 │
└─────────────────────────────────────────────────────┘
```

---

## Phase 1: Define Components

**Goal:** Create flat component structures and update GameObject

### Step 1.1: Define Components in `custom_types.h`

```c
// Add these AFTER the fixed-point definitions but BEFORE GameObject

// === COMPONENT FLAGS ===
#define COMP_ACTIVE    0x01  // Object is alive
#define COMP_TRANSFORM 0x02  // Has position
#define COMP_PHYSICS   0x04  // Has velocity/physics
#define COMP_RENDER    0x08  // Has sprite/should render

// === COMPONENT STRUCTURES ===

typedef struct {
    uint8_t x, y;  // Position in pixels
} TransformComponent;

typedef struct {
    fixed_n vx, vy;                    // Velocity
    fixed_n fractional_x, fractional_y; // Sub-pixel accumulator
} PhysicsComponent;

typedef struct {
    uint8_t sprite_index;  // Hardware sprite slot (0-39)
    uint8_t tile_index;    // Tile ID in VRAM
    uint8_t visible;       // Visibility flag
} RenderComponent;
```

### Step 1.2: Update GameObject in `game_object.h`

```c
// Replace the entire GameObject struct with:

struct GameObject {
    uint8_t id;          // Unique identifier (for debugging)
    uint8_t flags;       // Active components (bitmask)
    ObjectType tag;      // OBJ_BALL, OBJ_PIN (for logic branching)
    
    // Components embedded directly (data locality!)
    TransformComponent transform;
    PhysicsComponent physics;
    RenderComponent render;
    
    // Keep function pointer if needed for specific behaviors
    UpdateFunc update;
};
```

### Step 1.3: Remove Obsolete Types from `custom_types.h`

```c
// DELETE these old structs:
// typedef struct { ... } Ball;
// typedef struct { ... } Pin;
// typedef struct { ... } Wall;

// KEEP this enum (still useful for tagging):
typedef enum {
    OBJ_BALL,
    OBJ_PIN,
    OBJ_WALL  // Can add later
} ObjectType;
```

**Test:** Project should compile (with errors in other files - that's expected)

---

## Phase 2: Centralize Object Management

**Goal:** Move object pool and registries into `GameState`

### Step 2.1: Update `game_data.h`

```c
// Add ObjectManager struct BEFORE GameState definition:

typedef struct {
    // Object pool (all game objects live here)
    GameObject pool[MAX_GAME_OBJECTS];
    uint8_t total_count;  // Total active objects
    
    // Registries (indices into pool for fast type iteration)
    uint8_t ball_indices[MAX_BALLS];
    uint8_t ball_count;
    
    // Future: pin_indices[], wall_indices[], etc.
} ObjectManager;

// Update GameState:
typedef struct {
    System system;
    Graphics graphics;
    ObjectManager objects;  // ← ADD THIS
} GameState;
```

### Step 2.2: Update Graphics struct in `game_data.h`

```c
typedef struct {
    unsigned char *active_background_tilemap;
    unsigned char *active_background_tiledata;
    unsigned char *active_sprite_sheet;
    uint8_t next_sprite_id;  // Already exists
    uint8_t sprite_count;    // ← ADD THIS (tracks usage)
} Graphics;
```

### Step 2.3: Update `game_object.c`

**Remove static variables:**
```c
// DELETE these lines:
// static GameObject game_object_pool[MAX_GAME_OBJECTS];
// static uint8_t total_object_count = 0;
// static ObjectIndices indices;
```

**Update `go_init_manager()`:**
```c
void go_init_manager(void) {
    game.objects.total_count = 0;
    game.objects.ball_count = 0;
    // memset pool if needed for safety
}
```

**Update `go_spawn_object()`:**
```c
GameObject* go_spawn_object(ObjectType type) {
    if (game.objects.total_count >= MAX_GAME_OBJECTS) {
        return NULL; // Pool full
    }

    uint8_t pool_index = game.objects.total_count;
    GameObject* obj = &game.objects.pool[pool_index];
    
    // Clear the object
    obj->id = pool_index;
    obj->flags = COMP_ACTIVE;
    obj->tag = type;
    obj->update = NULL;
    
    // Register in type-specific registry
    if (type == OBJ_BALL && game.objects.ball_count < MAX_BALLS) {
        game.objects.ball_indices[game.objects.ball_count] = pool_index;
        game.objects.ball_count++;
    }
    
    game.objects.total_count++;
    return obj;
}
```

**Update `go_update_all()`:**
```c
void go_update_all(void) {
    // Iterate through ball registry
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
        uint8_t pool_index = game.objects.ball_indices[i];
        GameObject* obj = &game.objects.pool[pool_index];
        
        if ((obj->flags & COMP_ACTIVE) && obj->update != NULL) {
            obj->update(obj);
        }
    }
}
```

**Update `go_draw_all()`:**
```c
void go_draw_all(void) {
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
        uint8_t pool_index = game.objects.ball_indices[i];
        GameObject* obj = &game.objects.pool[pool_index];
        
        if ((obj->flags & (COMP_ACTIVE | COMP_RENDER)) == (COMP_ACTIVE | COMP_RENDER)) {
            DRAW_SPRITE(&obj->render, obj->transform.x, obj->transform.y);
        }
    }
}
```

**Update `go_return_ball()`:**
```c
GameObject* go_return_ball(uint8_t index) {
    if (index < game.objects.ball_count) {
        uint8_t pool_index = game.objects.ball_indices[index];
        return &game.objects.pool[pool_index];
    }
    return NULL;
}
```

### Step 2.4: Remove `ObjectIndices` from `game_object.h`

```c
// DELETE this typedef:
// typedef struct { ... } ObjectIndices;
```

**Test:** Project compiles, object manager works with centralized state

---

## Phase 3: Update Graphics System

**Goal:** Sprite allocation uses centralized tracking

### Step 3.1: Update `graphics.h` DRAW_SPRITE macro

```c
// Update to work with RenderComponent pointer:
#define DRAW_SPRITE(render_ptr, x, y) \
    move_sprite((render_ptr)->sprite_index, (x) + 8, (y) + 16)
```

### Step 3.2: Update `graphics.c` - Sprite Allocation

```c
GameSprite create_sprite(uint8_t tile_index) {
    GameSprite new_sprite;
    
    // Safety check: prevent overflow
    if (game.graphics.next_sprite_id >= 40) {
        // Handle error: wrap around or return invalid sprite
        new_sprite.sprite_index = 0xFF; // Invalid marker
        new_sprite.tile_index = tile_index;
        return new_sprite;
    }
    
    new_sprite.sprite_index = game.graphics.next_sprite_id;
    new_sprite.tile_index = tile_index;
    set_sprite_tile(game.graphics.next_sprite_id, tile_index);
    
    game.graphics.next_sprite_id++;
    game.graphics.sprite_count++;  // Track usage
    
    return new_sprite;
}
```

**Optional: Add sprite release function**
```c
void release_sprite(uint8_t sprite_index) {
    // For future pooling/reuse
    if (game.graphics.sprite_count > 0) {
        game.graphics.sprite_count--;
    }
}
```

**Test:** Sprites allocate correctly, no VRAM overflow

---

## Phase 4: Update Ball System

**Goal:** Balls use flat components instead of union

### Step 4.1: Update `ball.h`

```c
// Remove old Ball typedef
// Keep only:
GameObject* spawn_ball(uint8_t x, uint8_t y);
void update_ball(GameObject* obj);
void reset_all_balls(void);
```

### Step 4.2: Rewrite `ball.c` - Spawn Function

```c
GameObject* spawn_ball(uint8_t x, uint8_t y) {
    GameObject* obj = go_spawn_object(OBJ_BALL);
    if (obj == NULL) return NULL;
    
    // Configure components
    obj->flags |= (COMP_TRANSFORM | COMP_PHYSICS | COMP_RENDER);
    
    // Transform
    obj->transform.x = x;
    obj->transform.y = y;
    
    // Physics
    obj->physics.vx = 0;
    obj->physics.vy = 0;
    obj->physics.fractional_x = 0;
    obj->physics.fractional_y = 0;
    
    // Render
    GameSprite sprite = create_sprite(TILE_BALL);
    obj->render.sprite_index = sprite.sprite_index;
    obj->render.tile_index = sprite.tile_index;
    obj->render.visible = 1;
    
    // Assign update function
    obj->update = update_ball;
    
    return obj;
}
```

### Step 4.3: Rewrite `update_ball()`

```c
void update_ball(GameObject* obj) {
    // Quick validation
    if (!(obj->flags & COMP_PHYSICS)) return;
    
    // Handle collision (pass GameObject directly)
    handle_ball_pin_collision(obj);
    
    // Update position (pass GameObject directly)
    update_ball_position(obj);
}
```

### Step 4.4: Rewrite `reset_all_balls()`

```c
void reset_all_balls(void) {
    for (uint8_t i = 0; i < game.objects.ball_count; i++) {
        GameObject* obj = go_return_ball(i);
        if (obj == NULL) continue;
        
        // Reset position
        if (i < 8) {
            obj->transform.x = 10 + i * 8;
            obj->transform.y = 20;
        } else {
            obj->transform.x = 10 + (i - 8) * 8;
            obj->transform.y = 30;
        }
        
        // Reset physics
        obj->physics.vx = RANDOM_HORIZONTAL_VX[i];
        obj->physics.vy = 0;
        obj->physics.fractional_x = 0;
        obj->physics.fractional_y = 0;
    }
}
```

### Step 4.5: Delete `init_ball()` function

```c
// DELETE this entire function - logic is in spawn_ball() now
```

**Test:** Balls spawn, move, and reset correctly

---

## Phase 5: Update Physics System

**Goal:** Physics operates on GameObject components

### Step 5.1: Update `physics.h` signatures

```c
// Change ALL Ball* parameters to GameObject*
void update_ball_position(GameObject* obj);
void apply_impulse(GameObject* obj, fixed_n impulse_x, fixed_n impulse_y);
void check_ball_wall(GameObject* obj, Wall* w);
void handle_ball_pin_collision(GameObject* obj);
```

### Step 5.2: Rewrite `update_ball_position()` in `physics.c`

```c
void update_ball_position(GameObject* obj) {
    // Apply gravity
    obj->physics.vy += GRAVITY;
    if (obj->physics.vy > MAX_SPEED) {
        obj->physics.vy = MAX_SPEED;
    }
    
    // Accumulate sub-pixel movement
    obj->physics.fractional_x += obj->physics.vx;
    obj->physics.fractional_y += obj->physics.vy;
    
    // Apply integer movement
    obj->transform.x += (int8_t)(obj->physics.fractional_x >> FIXED_SHIFT);
    obj->transform.y += (int8_t)(obj->physics.fractional_y >> FIXED_SHIFT);
    
    // Keep fractional remainder
    obj->physics.fractional_x &= 0xFF;
    obj->physics.fractional_y &= 0xFF;
}
```

### Step 5.3: Rewrite `apply_impulse()`

```c
void apply_impulse(GameObject* obj, fixed_n impulse_x, fixed_n impulse_y) {
    if (!(obj->flags & COMP_PHYSICS)) return;
    
    obj->physics.vx += impulse_x;
    obj->physics.vy += impulse_y;
}
```

### Step 5.4: Update `handle_ball_pin_collision()`

```c
void handle_ball_pin_collision(GameObject* obj) {
    // Early exit if not moving down
    if (obj->physics.vy <= 0) return;
    
    // Get ball's bottom-middle point
    uint8_t ball_bottom_x = obj->transform.x + TILE_HALF_WIDTH;
    uint8_t ball_bottom_y = obj->transform.y + TILE_WIDTH;
    
    // Find tile coordinates
    uint8_t col = PIXEL_TO_GRID(ball_bottom_x);
    uint8_t row = PIXEL_TO_GRID(ball_bottom_y);
    
    // Bounds check
    if (col >= BACKGROUND_WIDTH_TILES || row >= BACKGROUND_HEIGHT_TILES) return;
    
    uint16_t tilemap_index = GET_TILE_INDEX(col, row);
    
    // Check for pin tile
    if (get_game_background_tilemap()[tilemap_index] != PIN_TILE_ID) return;
    
    // Calculate collision
    uint8_t pin_tile_x = GRID_TO_PIXEL(col);
    uint8_t pin_tile_y = GRID_TO_PIXEL(row);
    uint8_t pin_center_x = pin_tile_x + TILE_HALF_WIDTH;
    int8_t distance_x = ball_bottom_x - pin_center_x;
    
    // Horizontal bounds check
    if (distance_x < -(PIN_HALF_WIDTH) || distance_x > (PIN_HALF_WIDTH)) return;
    
    // Vertical distance check
    uint8_t pin_visual_top = pin_tile_y + PIN_HALF_WIDTH;
    int8_t vertical_dist = ball_bottom_y - pin_visual_top;
    if (vertical_dist < 0 || vertical_dist > TILE_HALF_WIDTH) return;
    
    // === COLLISION RESPONSE ===
    obj->physics.fractional_y = 0;
    
    if (obj->physics.vy > FIXED_QUARTER) {
        // BOUNCE
        obj->physics.vy = -(obj->physics.vy >> 1);
        
        if ((obj->physics.vx > 0 && distance_x < 0) || 
            (obj->physics.vx < 0 && distance_x > 0)) {
            obj->physics.vx = -(obj->physics.vx >> 1);
        } else {
            obj->physics.vx = (obj->physics.vx >> 1);
            if (distance_x > 0) {
                obj->physics.vx += FIXED_EIGHTH;
            } else if (distance_x < 0) {
                obj->physics.vx -= FIXED_EIGHTH;
            } else {
                obj->physics.vx += (obj->transform.y & 1) ? FIXED_EIGHTH : -FIXED_EIGHTH;
            }
        }
    } else {
        // ROLL
        obj->physics.vy = 0;
        obj->physics.vx += (distance_x >> 2);
        
        if (obj->physics.vx > MAX_ROLL_SPEED) obj->physics.vx = MAX_ROLL_SPEED;
        else if (obj->physics.vx < -MAX_ROLL_SPEED) obj->physics.vx = -MAX_ROLL_SPEED;
    }
}
```

### Step 5.5: Update `check_ball_wall()` (if used)

```c
void check_ball_wall(GameObject* obj, Wall* w) {
    if (obj->physics.vy > 0) {
        uint8_t ball_bottom = obj->transform.y + SPRITE_SIZE;
        
        if (ball_bottom >= w->y) {
            obj->transform.y = w->y - SPRITE_SIZE;
            obj->physics.fractional_y = 0;
            obj->physics.vy = -(obj->physics.vy >> 1);
            obj->physics.vx = obj->physics.vx - (obj->physics.vx >> 2);
            
            if (obj->physics.vy > -FIXED_QUARTER) {
                obj->physics.vy = 0;
                obj->physics.vx = 0;
                obj->physics.fractional_x = 0;
            }
        }
    }
}
```

**Test:** Physics works correctly with new component structure

---

## Phase 6: Update Pin System (Optional)

**Goal:** Create pin factory if you want pins as sprites

### Step 6.1: Update `pins.h`

```c
GameObject* spawn_pin(uint8_t x, uint8_t y);
// Delete old init functions
```

### Step 6.2: Create `spawn_pin()` in `pins.c`

```c
GameObject* spawn_pin(uint8_t x, uint8_t y) {
    GameObject* obj = go_spawn_object(OBJ_PIN);
    if (obj == NULL) return NULL;
    
    // Pins have transform + render (no physics)
    obj->flags |= (COMP_TRANSFORM | COMP_RENDER);
    
    obj->transform.x = x;
    obj->transform.y = y;
    
    GameSprite sprite = create_sprite(TILE_PIN);
    obj->render.sprite_index = sprite.sprite_index;
    obj->render.tile_index = sprite.tile_index;
    obj->render.visible = 1;
    
    return obj;
}
```

**Note:** Since your pins are currently background tiles, you may not need this yet.

---

## Phase 7: Update State Logic

**Goal:** States use new APIs correctly

### Step 7.1: Update `state_game_screen.c` - Init

```c
void init_game_screen(void) {
    DISPLAY_OFF;
    SPRITES_8x8;
    
    // Reset object manager
    go_init_manager();
    
    // Reset graphics tracking
    game.graphics.next_sprite_id = 0;
    game.graphics.sprite_count = 0;
    
    // Load assets
    set_sprite_sheet(PanchinkoTiles);
    set_game_background(PachinkoMap, PanchinkoTiles);
    
    // Spawn balls using new factory
    for (uint8_t i = 0; i < MAX_BALLS; i++) {
        uint8_t ball_x, ball_y;
        
        if (i < 8) {
            ball_x = 10 + i * 8;
            ball_y = 20;
        } else {
            ball_x = 10 + (i - 8) * 8;
            ball_y = 30;
        }
        
        GameObject* ball = spawn_ball(ball_x, ball_y);
        if (ball != NULL) {
            ball->physics.vx = RANDOM_HORIZONTAL_VX[i];
        }
    }
    
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}
```

### Step 7.2: Update is minimal (already done)

```c
void update_game_screen(void) {
    game.system.previous_keys = game.system.keys;
    game.system.keys = joypad();
    
    if ((game.system.keys & J_UP) && !(game.system.previous_keys & J_UP)) {
        reset_all_balls();
    }
    
    go_update_all();
    go_draw_all();
    vsync();
}
```

**Test:** Full game loop works with component system

---

## Phase 8: Clean Up

### Step 8.1: Remove Dead Code

- [x] Delete `Ball`, `Pin`, `Wall` typedefs from `custom_types.h` (keep if used for collision helpers)
- [x] Delete union and `data` member from GameObject
- [x] Remove any commented-out old code
- [x] Remove `init_ball()` and `init_pin()` functions

### Step 8.2: Add Helper Macros (Optional)

```c
// In game_object.h or custom_types.h
#define HAS_PHYSICS(obj)   ((obj)->flags & COMP_PHYSICS)
#define HAS_RENDER(obj)    ((obj)->flags & COMP_RENDER)
#define HAS_TRANSFORM(obj) ((obj)->flags & COMP_TRANSFORM)
#define IS_ACTIVE(obj)     ((obj)->flags & COMP_ACTIVE)
```

### Step 8.3: Delete Obsolete Files

- Consider removing `pins.c` if you're using background pins only
- Remove any unused include statements

---

## Testing Checklist

After each phase:

- [ ] Project compiles without errors
- [ ] Balls spawn at correct positions
- [ ] Balls fall with gravity
- [ ] Balls bounce off pins correctly
- [ ] Sprites render at correct positions
- [ ] No visual glitches or corruption
- [ ] Reset function works (J_UP)
- [ ] No crashes or freezes
- [ ] Memory usage reasonable (check ROM size)

---

## Performance Notes for Game Boy

1. **Cache GameObject pointers** in loops:
   ```c
   GameObject* obj = &game.objects.pool[i];
   obj->transform.x = 10;  // Good
   // NOT: game.objects.pool[i].transform.x = 10;  // Bad
   ```

2. **Component flags are fast** - bit checks are cheap:
   ```c
   if (obj->flags & COMP_PHYSICS) { ... }
   ```

3. **Data locality matters** - flat components in GameObject keep cache-friendly

4. **Registry iteration is optimal** - only loop through active balls, not entire pool

---

## Quick Reference: Before & After

### Before (Union-based)
```c
Ball* ball = &go_return_ball(i)->data.ball;
ball->vx = 100;
obj->x = ball->x;  // Position duplication!
```

### After (Component-based)
```c
GameObject* obj = go_return_ball(i);
obj->physics.vx = 100;
// No duplication - transform.x is single source
DRAW_SPRITE(&obj->render, obj->transform.x, obj->transform.y);
```

---

## Next Steps After Refactor

Once this is complete, you can:
- Add pin registry for sprite-based pins
- Add wall objects with OBJ_WALL tag
- Add particle effects (new component type)
- Add collision layers (new component)
- Add audio component (sound effects per object)
- Implement object pooling/reuse
- Add save/load system (all state in one struct!)

---

## Common Gotchas

⚠️ **Don't forget sprite offsets:** Game Boy sprites render at (x+8, y+16)  
⚠️ **Check component flags:** Always verify flags before accessing components  
⚠️ **Reset fractional values:** When changing physics, clear fractional_x/y  
⚠️ **VRAM sprite limit:** Only 40 hardware sprites available (check overflow)  
⚠️ **Registry capacity:** MAX_BALLS = 16 in game_object.h, don't exceed  

---

Good luck with the refactor! Take it one phase at a time and test thoroughly after each step.