# Chip-8 Emulator

This project is an implementation of a [chip-8](https://en.wikipedia.org/wiki/CHIP-8) using [SDL2](https://www.libsdl.org/) as the graphics library for the "display".

## Building the project

1. Install SDL2 library. In OS X, you can install SDL using ```brew install sdl2```.
2. make symlinks to SDL2 headers into the ```include``` folder, so the the local path to headers will be ```include/SDL2/```.
3. change  ```src/main.cpp``` to load the proper chip-8 game. (move game from ```chip8-programs``` folder to project root and use file name eg "pong2.c8")
4. use ```make``` in project root to build the project.
5. run ```./build/debug/play``` to run the emulator.
NOTE: all the keys (0-F) are mapped to each character on the keyboard (ie 0 is 0, C is C, etc.).
