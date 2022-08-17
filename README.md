# Chip-8 Emulator

This project is an implementation of a [chip-8](https://en.wikipedia.org/wiki/CHIP-8) using [SDL2](https://www.libsdl.org/) as the graphics library for the "display".

## Requirements

This project requires:

1. C++ compiler (clang or gcc), needs to support C++17
2. CMake >= 3.22.3 (lower the version in `CMakeLists.txt` if needed)
3. Conan >= 1.49

## Building the project

1. Create a `build` folder and change directory to `build`.
2. Install dependencies (SDL2) with `conan install .. --build missing`.
3. run `cmake ..` to generate the proper build files.
4. run `make` build the project.

If you are using VSCode, it will detect the `CMakeLists.txt` and configure the project for you.

### Running the executable

```Bash
build/bin/chip-8 <path-to-rom>
```

### Copyright

see LICENSE.md for project license.

**All the ROM files are copyrighted by their respective owner and I do NOT own any rights over their work. Please refer to the author.**
