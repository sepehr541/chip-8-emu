#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "display.hpp"
#include "chip8.hpp"
#include "displaydriver.hpp"
#include "keyboarddriver.hpp"

// arg[0] = "chip-8"
// arg[1] = path to ROM file
#define MIN_ARG_COUNT 2

int main(int argc, char **argv)
{
    if (argc < MIN_ARG_COUNT)
    {
        std::cout << "Error: Missing path to ROM file" << std::endl;
        return 1;
    }

    // init Chip8
    Chip8 chip8;
    chip8.initialize();
    chip8.loadGame(std::string(argv[1]));

    // init display
    SDL_Init(SDL_INIT_VIDEO);
    Display display("CHIP-8", Chip8::DIS_WIDTH, Chip8::DIS_HEIGHT, 8);

    // initialize drivers
    DisplayDriver displayDriver(display);
    KeyboardDriver keyboardDriver(chip8);
    
    bool quit = false;
    // handle events
    while (!quit)
    {
        quit = keyboardDriver.handleEvents();
        
        chip8.emulateCycle();

        if (chip8.getRefreshFlag())
        {
            displayDriver.updateDisplay(chip8.getPixels());
        }
    }

    // clean up
    SDL_Quit();

    return 0;
}