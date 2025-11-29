# TODO

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

- [ ] Ball array and pin array should have a count of 20.
  -[ ] Ball and pin arrays need handling functions to ensure no overflow or overwrites take place
    - [ ] Re-use slot ball despawn order, prioritise which can be despawned if too many
- [ ] Handling for sprite_index, no more than 39. Will need to reassign indices to corresponding object (or inverse)

### Ball collision improvements

**roll**
- [x] Add a little bit of roll to the balls on collision with pins
- [ ] Add slanted walls balls can roll down!
**bounce**
- [ ] Add a bit of randomness to balls on bounce (very small amount)

**Collision detection optimisations**
- [ ] Wall collision can be checked with masks, as nothing is behind the wall.
  Essentially we are drawing an area of the game that is off bounds
- [x] Balls could be flagged collision_enabled to false then filtered out of the checking method
- [x] Balls could only check for any distance_y below them as bouncing back up would be rare...
- [x] Think about ball on ball collisions, could the sprite size be reduced?

**Housekeeping**
- [x] Clean up: code review; remove magic numbers; add good comments; Write up some documentation...

**Score and "window" sprites**
- [ ] Add a way of scoring... move the game beyond a tech demo...
  Number of balls, ball speed, ball weight change? Things on map
  and things that can be controlled wiht buttons would be cool
  Even power ups wuould be so good, can be showing with a window bg