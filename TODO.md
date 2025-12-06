# TODO

## **06/12/2025**

Reset focus after Game Object and Game State architecture.

### **Title screen with text and state transition**

- [ ] Create title screen BG graphics
- [ ] Build font tile-set
- [ ] Function to convert string into text @ tile_index on screen. Can parse newline and other chars in string
- [ ] On press start transition into Game State, check VRAM and data structures for cleanliness

### **Game Object Saftey**

- [ ] On create of Game Object, each type needs safety handling to prevent too many being created.
- [ ] Function to clean out game data on state transition

### **Graphics Safety**

- [ ] Read VRAM for usage. Use this to build safety and cleaning for state transitions.

### **New "physics"**

- [ ] Migrate collision tile checker to physics.c/.h
- [ ] Improve collision tile checker function to pick up contact within multiple tiles
- [ ] 45 degree walls vs ball collision implementation

**Tweaks**

- [ ] Add a bit of randomness to balls on bounce (very small amount)

---

## DONE:


**01/12/25**

- ~~Game states need properly implementing.~~
  - **gamestate .c/.h** create functions that handle state changes, memory init and cleanup
  - **`state_game` & `state_title` .c/.h** main in-game and title screen functions
- Expand on collisions and interactions by returning the tile IDs on the background and applying behaviour appropriate for those tiles.
- build input system that is scalable.
- **Measure time between any two points** in the code execution would be useful.
Optimisation will need looking at again.

- [x] Draw a plot point at the x, values of balls and pins to determine if x,y is on the top left.
    - [x] I'm thinking some decent debug functions. Maybe I can pass a gameobject
- [x] Extract pin positions from background data and instantiate the pins
- [x] States. This lets me switch out to a different scene pretty much. Like a different screen or level.
    I'm thinking that a state has an initialisation function and a loop function.
    I can then seperate the generic part of the loop. Maybe setting a proxy value for inputs depending on state.
    Basically if there's an enum or something for each state. So for input it would be checking
    if(STATE_PROXY_FOR_UP) or actually switch CASE that 
- [x] Create function to handle balls and pins loop with parameter for frame-skip
  - [x] BETTER: split into input, physics, and drawing so can be optimised seperately
- [x] Apply bounce to ball function, used to add force to ball depending on where it hits the pin
= [x] Create debug functionality to count instruction or measure performance to find bottlenecks


## **21-11-25 - TODO:**

### Pin spawing
- [x] Extract pin positions from background data and instantiate the pins
  - [x] disable sprites

### Game object memory management 

I need a place to store references to all the data object, their indices in memory, and aliases

- [x] Ball array and pin array should have a count of 20.


### Ball collision improvements

**roll**

- [x] Add a little bit of roll to the balls on collision with pins


**Collision detection optimisations**

~~- [ ] Wall collision can be checked with masks, as nothing is behind the wall. Essentially we are drawing an area of the game that is off bounds~~
- [x] Balls could be flagged collision_enabled to false then filtered out of the checking method
- [x] Balls could only check for any distance_y below them as bouncing back up would be rare...
- [x] Think about ball on ball collisions, could the sprite size be reduced?

**Housekeeping**

- [x] Clean up: code review; remove magic numbers; add good comments; Write up some documentation...
- [x] 06/11/25 - I am officially beginning this project. The first goal is to create a ball with gravity that can hit a wall and bounce until it's energy has depleted.

- [x] Started on 18/11/25 - My goal now is to place 10 pins in the game and have a button spawn balls at the top. The balls should drop down through the pins with good physics. I am allowing some time to play around with a title screen to test graphics out.

**update - 21/11/25**

  - [x] I have got the balls and pins working, collision is janky and performance is somewhat of an issue. I'm going to revise the
  collision method used, this bounding box thing is too heavy on the calcs. Also it's not reliable.

**update - 23/11/25:**

*See TODO.md list*

Physics is working nicely. Well optimised. Goals now:
- [x] Get a proper game screen with all the pins hooked up
- [x] Load in background and store array of pin locations
- [x] Use pin locations to spawn Pin structs in
~~Define zones on map, debug draw them~~
~~Hook up buttons to a ball firing system~~
~~Create diagnal walls with bounce logic~~
~~Balls will need a seperate collision mode:~~
~~Collide with wall mode (after being fired out) ~~
~~Collision mode defined by zones on the map~~

To achieve a game where balls are fired out on button press, collide with exit walls,
drop through the pins and to the bottom of the screen. Pin and wall positions are pulled
via the tile ID on the map data. Get some debug functions for drawing zones. 

**update - 29/11/25:**

The game is heading a few different directions from here. This is re-writing goals
for focus.

- [x] Better game state structure and switching