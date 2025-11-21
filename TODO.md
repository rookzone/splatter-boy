## TODO


- [ ] Draw a plot point at the x, values of balls and pins to determine if x,y is on the top left.
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