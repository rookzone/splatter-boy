# Splatter Boy

## About

A fun little project where I mess around with C and GBDK. Attempt at a pachinko game with decent ball physics

## Goals and progress
- [x] 06/11/25 - I am officially beginning this project. The first goal is to create a ball with gravity that can hit a wall and bounce until it's energy has depleted.

- [x] Started on 18/11/25 - My goal now is to place 10 pins in the game and have a button spawn balls at the top. The balls should drop down through the pins with good physics. I am allowing some time to play around with a title screen to test graphics out.

**update - 21/11/25**
  - [x] I have got the balls and pins working, collision is janky and performance is somewhat of an issue. I'm going to revise the
  collision method used, this bounding box thing is too heavy on the calcs. Also it's not reliable.

**update - 23/11/25:**  
*See TODO.md list*
Physics is working nicely. Well optimised. Goals now:
- [ ] Get a proper game screen with all the pins hooked up
- [ ] Load in background and store array of pin locations
- [ ] Use pin locations to spawn Pin structs in
- [ ] Define zones on map, debug draw them
- [ ] Hook up buttons to a ball firing system
- [ ] Create diagnal walls with bounce logic
- [ ] Balls will need a seperate collision mode:
- [ ] Collide with wall mode (after being fired out) 
- [ ] Collision mode defined by zones on the map

To achieve a game where balls are fired out on button press, collide with exit walls,
drop through the pins and to the bottom of the screen. Pin and wall positions are pulled
via the tile ID on the map data. Get some debug functions for drawing zones. 

## Fun?


**Project stuff**
- Check out licenses to use for distribution and tools used to make this.
- Wiki / Docs
- Physics engine for GB export?
- Asm optimisation
- Versioning!

## Links to dev tools

https://gameboy.prodigle.dev

https://github.com/systemoflevers/image_to_gb

## Programming styles

C styles:
https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide
https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## Setup and compile

1. Download GBDK 2020 and copy the contents into the repo. This will ensure all the required gbdk files are in the directory as the binaries and documents are not included *Note: check out licenses*

2. Use make

Just type `make`

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