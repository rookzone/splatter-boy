# Splatter Boy

## About

A fun little project where I mess around with C and GBDK. Attempt at a pachinko game with decent ball physics.

This is in __VERY EARLY__ stages.

## Current Features

- **Custom 8.8 Fixed-Point Physics** Implements a fixed-point number system for sub-pixel movement and velocity
- **Ball Physics:** Includes gravity, maximum speed enforcement, and functions for initial ball launch and instant velocity impulse application.
- **Ball-Pin Collision:** Logic supports two distinct interactions based on vertical velocity: **high-speed bounce** with velocity retention and **low-speed rolling** along the pin surface.
- **Background-Mapped Collision:** Pin collision is detected by reading the background tilemap (`PIN_TILE_ID`), allowing for level designs where pins are part of the level geometry rather than individual sprites.
- **Game State Management:** State defined for title and game screens

## Setup dev environment

Dev environment is Win11 with VSCode using Make. Compiler is LCC.exe included with GBDK2020.

_**note**: These are steps for Windows. For linux GBDK will need installing and a Makefile creating to compile. There is a `Makefile_linux` provided, but this might not be correct._

Follow these steps to get started:

1. `git clone https://github.com/rookzone/splatter-boy.git`
2. `cd splatter-boy`
3. Make sure you are in the splatter-boy workspace. Open VSCode at that folder.
4. Download or install [GBDK 2020](https://gbdk.org) and copy the contents into the repo.
5. Install [make for windows](https://gnuwin32.sourceforge.net/packages/make.htm)
6. Download [BGB emulator](https://bgb.bircd.org) and place in `emu/BGB`
7. `make` and it will create the .gb file in "emu/BGB"
8. Use below in .vscode/tasks.json. This will set it up to build and then launch straight into BGB on CTRL+SHIFT+B

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

## Structure

![Basic structure flow](simple-overview-flow.png)

## Programming styles

**C styles**:

https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide

https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## Current focus / goals

### Code

**01/12/25**

- Game states need properly implementing.
  - **gamestate .c/.h** create functions that handle state changes, memory init and cleanup
  - **`state_game` & `state_title` .c/.h** main in-game and title screen functions

- Expand on collisions and interactions by returning the tile IDs on the background and applying behaviour appropriate for those tiles.

- build input system that is scalable.

- **Measure time between any two points** in the code execution would be useful.
Optimisation will need looking at again.

Play some sound effects. Sound system.

### Project

- Structure diagram (Show how GameState works, memory pointers and arrays for data, custom types, engine functionalities, and state specific code and the loop) - **done** https://plantuml.com diagrams added.
- Wiki / Docs
- Possible in-line asm optimisations
- Versioning


## Links to dev tools

### Graphics

Community tools for creating tilesets and background maps:

[Game Boy Tile Designer](http://www.devrs.com/gb/hmgd/gbtd.html) - tilesets

[Game Boy Map Builder](http://www.devrs.com/gb/hmgd/gbmb.html) - background maps

This is super useful for turning images into PNGs compatible with Game Boy

[Image to GB](https://github.com/systemoflevers/image_to_gb)

Check out png2Asset in gbdk bin directory. Pass an image (Generated from above).
Will output a tileset and background map that is ready to use.

### Sounds

[example dev sofware](example.com)

## AI usage

There is no copy/paste AI code in this project (except the Makefiles... for now...). AI has been used to suggest documentation, provide a basic review of the code, and assist with comments and commit messages. This is a concious choice by myself, AI code is ok so long as the contributer understands what the code does.

## License

This project uses the MIT License. Any third-party tools including GBDK and png2asset have their respective licenses are provided in the /licenses directory.
