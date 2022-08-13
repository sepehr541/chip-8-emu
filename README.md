# Chip-8 Emulator

This project is an implementation of a [chip-8](https://en.wikipedia.org/wiki/CHIP-8) using [SDL2](https://www.libsdl.org/) as the graphics library for the "display".

## Requirements

This project requires:

1. C++ compiler
2. CMake
3. Conan

## Building the project

1. Create a `build` folder and change directory to `build`.
2. Install dependencies (SDL2) with `conan install .. --build missing`.
3. run `cmake ..` to generate the proper build files.
4. run `make` build the project.

### Running the executable

The executable is located at `build/bin/chip-8`.
NOTE: all the keys (0-F) are mapped to each character on the keyboard (ie 0 is 0, C is C, etc.).
