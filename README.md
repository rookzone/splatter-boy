# Splatter Boy

## About

A fun little project where I mess around with C and GBDK. Attempt at a pachinko-like "game" with decent ball physics.

This is in **_VERY EARLY_** stages.

## Current Features

- **Custom 8.8 Fixed-Point Physics** Implements a fixed-point number system for sub-pixel movement and velocity
- **Ball Physics:** Includes gravity, collision, bounce, rolling, and sub-pixel precision
- **Ball-Pin Collision:** Logic supports two distinct interactions based on vertical velocity: **high-speed bounce** with velocity retention and **low-speed rolling** along the pin surface.
- **Background-Mapped Collision:** Pin collision is detected by reading the background tilemap (`PIN_TILE_ID`), allowing for level designs where pins have "built-in" collision. This saves VRAM and a LOT of CPU
- **Game State Management:** Finite state machine implementation and centralised game data structure. Allows for easy reset, save state, and change state (i.e. screen, level)

## Setup Developer Environment

Dev environment is Win11 with VSCode using Make. Compiler is LCC.exe included with GBDK2020.

_**note**: These are steps for Windows. The Makefile should work on Linux in theory, as long as LCC is in PATH, but I have not been able to test this yet. I have no reccomendations for an emulator on Linux, but I'm sure there are plenty of accurate ones with debug features_

Follow these steps to get started:

1. `git clone https://github.com/rookzone/splatter-boy.git`
2. `cd splatter-boy`
3. `code .` Open VSCode - make sure you are in the splatter-boy workspace (_check with `pwd`_).
4. Download or install [GBDK 2020](https://gbdk.org) (v4.4.0) and copy the contents into the root.
5. Install [make for windows](https://gnuwin32.sourceforge.net/packages/make.htm)
6. Download [BGB emulator](https://bgb.bircd.org) and place in `emu/BGB` (_BGB is used as it has certain debugging features that interact with GBDK_)
7. `make` and it will create the .gb file in the `build/` directory
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

### Execution Flow

![Basic structure flow](docs/simple-overview-flow.png)

### Data Layout

![data diagram](docs/data-storage.png)

## Programming Styles

**Functions and Variables**

*snake_case, lower-case*

e.g 

`update_ball_physics`
`uint_8 ball_center_position`

**Macros and Defines**

*SNAKE_CASE, CAPITAL*

e.g.

`DRAW_SPRITE`
`MAX_GAME_OBJECTS`

**Structs, Typedefs, Enum, Union - "Custom Types"**

PascalCase

e.g.

`GameObject`
`GameSprite`
`GameState`
`Game`
`Ball`

**C styles**:

https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide

https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## Current Focus

### Code

- Font loading and text display
- build encapsulated input system
- **Measure time between any two points** in the code execution would be useful.
Optimisation will need looking at again.
- Sound system

### Project

- ~Structure diagram (Show how GameState works, memory pointers and arrays for data, custom types, engine functionalities, and state specific code and the loop)~ - **done** https://plantuml.com diagrams added.
- Wiki / Docs
- Possible in-line asm optimisations
- Versioning

## Dev Tools

Useful command to combine source code into a single document.

```powershell
Get-ChildItem -Recurse -File | Where-Object { $_.Name -ne "combined.txt" } | ForEach-Object { Get-Content $_.FullName -Raw } | Out-File combined.txt -Encoding utf8
```

### Graphics Tools

Community tools for creating tilesets and background maps:

[Game Boy Tile Designer](http://www.devrs.com/gb/hmgd/gbtd.html) - tilesets

[Game Boy Map Builder](http://www.devrs.com/gb/hmgd/gbmb.html) - background maps

This is super useful for turning images into PNGs compatible with Game Boy

[Image to GB](https://github.com/systemoflevers/image_to_gb)

Download the image in .bin format. b2c.py can convert the .bin into .c and .h files

Check out png2Asset in gbdk bin directory. Pass an image (Generated from above).
Will output a tileset and background map that is ready to use.

### Sound Tools

 ...

## AI Usage

There is no copy/paste AI code in this project (except the Makefiles...). AI has been used for documentation, comments, commit messages, and code reviews. Assuming anyone contributes, AI code is fine as long as they understand what it does. An issue I found with AI and a low-level project like this is that AI tends to add a lot of extraneous code, which for a performance critical project like this, becomes a problem quite rapidly.

## License

This project uses the MIT License. Any third-party tools including GBDK and png2asset have their respective licenses are provided in the `licenses` directory.
