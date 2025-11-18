# Splatter Boy

## TODO

= [] Add collision logic to pins (basic like the wall) and check that bounding box is correct 
- [] Extract pin positions from background data and instantiate the pins
- [] Create function to handle balls and pins loop with parameter for frame-skip
  - [ ] BETTER: split into input, physics, and drawing so can be optimised seperately
- [ ] Apply bounce to ball function, used to add force to ball depending on where it hits the pin

- [ ] States. This lets me switch out to a different scene pretty much. Like a different screen or level.

## About

A fun little project where I mess around with C and GBDK. Attempt at a pachinko game with decent ball physics

## Goals and progress
= [x] 06/11/25 - I am officially beginning this project. The first goal is to create a ball with gravity that can hit a wall and bounce until it's energy has depleted.

- [ ] Started on 18/11/25 - My goal now is to place 10 pins in the game and have a button spawn balls at the top. The balls should drop down through the pins with good physics. I am allowing some time to play around with a title screen to test graphics out.

## Links to dev tools

https://gameboy.prodigle.dev

** HOWEVER THIS WORKS, I WANT IT TO MAKE GFX**

https://github.com/systemoflevers/image_to_gb

## Programming styles

C styles:
https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide
https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## Setup and compile

1. Download GBDK 2020 and copy the contents into the repo. This will ensure all the required gbdk files are in the directory, as not everything copies over due to .gitgnore

2. Use make

Just use make.

**.vscode tasks.json**

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