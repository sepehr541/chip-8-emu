#include <SDL.h>
#include "display.h"
#include "chip8.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    // init display
    SDL_Init(SDL_INIT_VIDEO);
    Display* display = new Display(64, 32, 8);

    // init Chip8
    Chip8 chip8;
    chip8.initialize();
    chip8.loadGame("pong2.c8");

    // emu loop
    display->loop(&chip8);

    // clean up
    delete display;
    SDL_Quit();

    return 0;
}