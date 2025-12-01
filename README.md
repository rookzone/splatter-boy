# Splatter Boy

## About

A fun little project where I mess around with C and GBDK. Attempt at a pachinko game with decent ball physics

## Current goals, progress, and todo

**01/12/25**

States need properly implementing.
state_game (main in-game) and state_title (title screen) need doing, with easy switching.

establish a decent input system that is scalable.

**Measure resource between any two points** in the code execution would be useful.
Optimisation will need looking at again.

Sound stuff.

**Project stuff todo**

- Wiki / Docs
- Possible in-line asm optimisations
- Versioning

## Links to dev tools

Super useful for creating tilesets and background maps:

[Game Boy Tile Designer](http://www.devrs.com/gb/hmgd/gbtd.html)
[Game Boy Map Builder](http://www.devrs.com/gb/hmgd/gbmb.html)

This is super useful for turning images into PNGs compatible with Game Boy

[Image to GB](https://github.com/systemoflevers/image_to_gb)

Check out png2Asset in gbdk bin directory. Pass an image (Generated from above).
Will output a tileset and background map that is ready to use.

## Programming styles

C styles:
https://www.cs.umd.edu/~nelson/classes/resources/cstyleguide
https://www.kernel.org/doc/html/v4.10/process/coding-style.html

## AI usage

Very little to no AI code. AI seems to suck at C, especially optimised C for GB dev.
I have used it on occasion to recommend reading materials and discuss ideas.
The idea was to not use it at all on this project. This is my personal choice, I'm not bothered what anyone else does.

## Setup and compile

Dev environment is Win11 and VSCode. Will probably be fine on linux but may need some tweaks in Makefile.
_note: at some point add in .env or equivalent to allow easy cross-platform dev_

1. Download [GBDK 2020](https://gbdk.org) and copy the contents into the repo. 

2. Use [make for windows](https://gnuwin32.sourceforge.net/packages/make.htm)

3. Download [BGB emulator](https://bgb.bircd.org) and place in `emu/BGB`

4. In .vscode create tasks.json with content below. Then build and run is ctrl+shift+b.

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