# Splatter Boy

## TODO SUMMARY

- [ ] Draw a plot point at the x, values of balls and pins to determine if x,y is on the top left.
- [ ] Extract pin positions from background data and instantiate the pins
- [ ] Create function to handle balls and pins loop with parameter for frame-skip
  - [ ] BETTER: split into input, physics, and drawing so can be optimised seperately
- [ ] Apply bounce to ball function, used to add force to ball depending on where it hits the pin

**Clean up repo**

Remove all GBDK files not required. Should only need includes and compiler

## About

A fun little project where I mess around with C and GBDK. Attempt at a pachinko game with decent ball physics

= [x] 06/11/25 - I am officially beginning this project. The first goal is to create a ball with gravity that can hit a wall and bounce until it's energy has depleted.

## Links to dev tools

https://gameboy.prodigle.dev

** HOWEVER THIS WORKS, I WANT IT TO MAKE GFX**

https://github.com/systemoflevers/image_to_gb

## Styles

C styles:
https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide
https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## Setup and compile

1. Download GBDK 2020 and copy the contents into the repo. This will ensure all the required gbdk files are in the directory, as not everything copies over due to .gitgnore

2. Use make

Just use make. Check out 

```JSON
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build GameBoy ROM",
      "type": "shell",
      "command": "make",
      "group": {
        "kind": "build",
        "isDefault": false
      },
      "problemMatcher": [],
      "options": {
        "cwd": "${workspaceFolder}"
      }
    },
    {
      "label": "Run in Emulator",
      "type": "shell",
      "command": "powershell",
      "args": [
        "-Command",
        "& '${workspaceFolder}/emu/BGB/bgb.exe' '${workspaceFolder}/build/splatter-boy.gb'"
      ],
      "dependsOn": "Build GameBoy ROM",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": [],
      "options": {
        "cwd": "${workspaceFolder}"
      }
    }
  ]
}

```


## TODO

Collision logic required. Pin bounce physics required, knowledge of coordinate here:

<img width="700" height="526" alt="image" src="https://github.com/user-attachments/assets/dc91593f-e0c0-4112-9470-0ac3549d81c6" />
