#include <cstdlib>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "chip8.hpp"

void Chip8::initialize()
{
    pc = 0x200; // Program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;     // Reset stack pointer

    // Load fontset
    for (int i = 0; i < 80; ++i)
        memory[0x50 + i] = chip8_fontset[i];
}

void Chip8::emulateCycle()
{
    clock.tick();
    refreshFlag = false;
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode & Execute Opcode
    ((*this).*(OpTable[(opcode & 0xF000u) >> 12u]))();

    if (clock.ticked)
    {
        decTimers();
        refreshFlag = true;
    }
}

void Chip8::decTimers()
{
    // Update timers
    delay_timer = std::max(delay_timer - 1, 0);
    sound_timer = std::max(sound_timer - 1, 0);
}

void Chip8::loadGame(std::string game)
{
    std::ifstream myfile;
    myfile.open(game, std::ios::binary);
    if (myfile.is_open())
        myfile.read((char *)&memory[0x200], 3584);
    else
        std::cout << "error loading game: " << game << std::endl;
    myfile.close();
}

bool Chip8::getRefreshFlag()
{
    return refreshFlag;
}

std::vector<uint32_t>& Chip8::getPixels()
{
    return this->pixels;
}

void Chip8::setKey(int index, uint8_t state)
{
    if (index > 0)
        keys[index] = state;
}
