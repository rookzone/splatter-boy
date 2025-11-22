## TODO


- [x] Draw a plot point at the x, values of balls and pins to determine if x,y is on the top left.
    - [ ] I'm thinking some decent debug functions. Maybe I can pass a gameobject
- [ ] Extract pin positions from background data and instantiate the pins
- [x] States. This lets me switch out to a different scene pretty much. Like a different screen or level.
    I'm thinking that a state has an initialisation function and a loop function.
    I can then seperate the generic part of the loop. Maybe setting a proxy value for inputs depending on state.
    Basically if there's an enum or something for each state. So for input it would be checking
    if(STATE_PROXY_FOR_UP) or actually switch CASE that 
- [x] Create function to handle balls and pins loop with parameter for frame-skip
  - [x] BETTER: split into input, physics, and drawing so can be optimised seperately
- [x] Apply bounce to ball function, used to add force to ball depending on where it hits the pin
= [x] Create debug functionality to count instruction or measure performance to find bottlenecks


**21-11-25** - todo:

- [ ] Extract pin positions from background data and instantiate the pins
  - [ ] disable sprites
- [ ] Add a little bit of roll to the balls on collision with pins
- [ ] Add a bit of randomness to balls on bounce (very small amount)

- [ ] Add slanted walls balls can roll down!
  Wall collision can be checked with masks, as nothing is behind the wall.
  Essentially we are drawing an area of the game that is off bounds
- [ ] Add a way of scoring... move the game beyond a tech demo...
  Number of balls, ball speed, ball weight change? Things on map
  and things that can be controlled wiht buttons would be cool
  Even power ups wuould be so good, can be showing with a window bg