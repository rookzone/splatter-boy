Here is a high-level summary of the refactor changes in Markdown format. You can use this for documentation, commit messages, or team explanations.

-----

# Refactor Summary: Data-Oriented Design & Component System

This refactor transitions the engine from an Object-Oriented style (using unions and function pointers) to a **Data-Oriented Design (DOD)**. The goal was to centralize state, remove expensive function pointers, and create a modular component system suitable for the Game Boy (Z80) hardware.

## 1\. Centralized Game State

**Goal:** Remove hidden static variables and create a "Single Source of Truth."

  * **Before:** Game state was scattered. `game_object.c` held static pools and counters hidden from the rest of the application.
  * **After:** All game data is now contained within the `GameState` struct in `game_data.h`. This allows any system to access the object pool, inputs, or graphics state directly.

<!-- end list -->

```c
// game_data.h (After)
typedef struct {
    System system;
    Graphics graphics;
    ObjectManager objects; // Contains the pool and registries
} GameState;
```

## 2\. Component-Based Game Objects

**Goal:** Decouple logic from data and allow objects to share features (Physics, Rendering) without complex unions.

  * **Before:** Objects used a `union` to store specific data (e.g., `Ball`, `Pin`) and relied on `ObjectType` enums to know how to read the memory.
  * **After:** Objects are now containers for **Components** (`Transform`, `Physics`, `Renderer`). A system of **Bitwise Flags** determines which components are active.

| Feature | Before (Union/OO) | After (Component) |
| :--- | :--- | :--- |
| **Data Storage** | `obj->data.ball.vx` | `obj->physics.vx` |
| **Definition** | `union { Ball ball; Pin pin; }` | `TransformComponent transform;` `PhysicsComponent physics;` |
| **Activation** | Implicit based on Type | `obj->flags |= PHYSICS_ACTIVE;` |

## 3\. Removal of Function Pointers

**Goal:** Improve performance on the Z80 processor by removing overhead.

  * **Before:** Each object carried a function pointer (`void (*update)(GameObject *obj)`). The update loop had to dereference this pointer every frame.
  * **After:** The update loop iterates through specific object registries (e.g., `ball_indices`) and calls the specific function (`update_ball`) directly. This is significantly faster on embedded hardware.

<!-- end list -->

```c
// game_object.c (After)
// Direct call - No function pointer overhead
update_ball(obj); 
```

## 4\. Physics & System Decoupling

**Goal:** Make systems generic so they work on *any* object, not just "Balls."

  * **Before:** Physics functions required a specific `Ball*` struct (e.g., `update_ball_position(Ball* ball)`). You couldn't apply gravity to a "Pin" or "Powerup" without writing new code.
  * **After:** Physics functions accept a `GameObject*` and operate on the generic `PhysicsComponent`. Any object with the `PHYSICS_ACTIVE` flag can now use gravity, velocity, and collision logic.

<!-- end list -->

```c
// physics.c (After)
// Works on balls, enemies, debris, etc.
void update_ball_position(GameObject* obj) {
    obj->physics.vy += GRAVITY;
    // ...
}
```

## 5\. Explicit "Registry" Management

**Goal:** Optimize loops by skipping inactive or irrelevant objects.

  * **Change:** The `ObjectManager` now explicitly tracks arrays of indices for specific types (e.g., `ball_indices[]`).
  * **Benefit:** Instead of looping through all 40 objects (many of which might be static walls), the update loop only iterates through the 1-20 active balls.