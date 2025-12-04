# Generic Game Objects ECS

**04/12/2025**

The game now manages generic objects centrally, which allows creation of any game object with it's own unique attribute structure, and it's execution code. This has improved seperation of concerns by moving out a lot of object specific code into it's appropriate file.

The state_game_screen code is now a lot less messy, and a lot of the ball initilisation and updating takes place in ball.c.

The game objects themselves are maintained in another file, which becomes a location to fetch game object data.

this will be improved with more functions to manage the game object pool, to manipulate objects in certain ways. This also lays the ground work for encapsulating the game_data structure further for things like input, sound etc.

## Object Management Architecture (ECS Pattern)

The project now uses a Game Object Manager based on a highly optimized, custom Entity-Component-System (ECS) pattern, designed specifically for the Game Boy's limited resources.

1. The Entity (GameObject)
All active game entities (balls, pins, etc.) are stored in a fixed-size array called the game_object_pool (the Entity Pool).

The generic GameObject struct acts as the Entity (or container). It holds the basic, mandatory data: screen position (x, y), active status, a sprite reference (GameSprite), and crucially, a function pointer (UpdateFunc update).

2. The Components (Specific Data)
Specific data for each type of object (e.g., vx, vy, sub_x, sub_y for a ball) is stored in a union data member within the GameObject.

The Ball struct is therefore the Component, holding the unique state required for the physics system to operate.

3. The System (Update Logic)
The update function pointer is assigned the address of a type-specific logic function (e.g., update_ball in ball.c). This separates the generic container from its unique behavior.

Functions like update_ball are the Systems that process the Component data. These functions contain the logic for physics integration and collision resolution.

4. Execution & Optimization
The system is optimized for CPU efficiency by avoiding a costly linear search over the entire pool.

A structure called ObjectIndices maintains a separate, compact array of pool indices (ball_indices) only for active objects of a specific type.

The main update loop, go_update_all(), iterates only over these small index arrays. For each index, it retrieves the GameObject from the main pool and calls its assigned update(obj) function pointer.

This approach ensures fast, targeted processing, as the system knows exactly which objects to update without wasting cycles.

## Visualization

The flow of data and control is highly centralized within this architecture.

Creation Flow (PlantUML Diagram):
![Creation Flow](ecs-init-flow.png)


Game Loop Flow (PlantUML Diagram):
![Game Loop Flow](loop-flow.png)
