#include <SDL.h>
#include "display.hpp"
#include "chip8.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <backward.hpp>

// arg[0] = "chip-8"
// arg[1] = path to ROM file
#define MIN_ARG_COUNT 2

int main(int argc, char **argv)
{

    // init display
    SDL_Init(SDL_INIT_VIDEO);
    Display display(Chip8::DIS_WIDTH, Chip8::DIS_HEIGHT, 8);

    if (argc < MIN_ARG_COUNT)
    {
        std::cout << "Error: Missing path to ROM file" << std::endl;
        return 1;
    }
    
    // init Chip8
    Chip8 chip8;
    chip8.initialize();
    chip8.loadGame(std::string(argv[1]));

    // emu loop
    display.loop(chip8);

    // clean up
    SDL_Quit();

    return 0;
}