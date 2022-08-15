#include <SDL.h>
#include "display.hpp"
#include "chip8.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    // init display
    SDL_Init(SDL_INIT_VIDEO);
    Display display(Chip8::DIS_WIDTH, Chip8::DIS_HEIGHT, 8);

    // init Chip8
    Chip8 chip8;
    chip8.initialize();
    chip8.loadGame("pong2.c8");

    // emu loop
    display.loop(chip8);

    // clean up
    SDL_Quit();

    return 0;
}