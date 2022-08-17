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

void Chip8::drawSprite(uint16_t opcode)
{
    uint8_t x = getRegisterAtX() % DIS_WIDTH;
    uint8_t y = getRegisterAtY() % DIS_HEIGHT;
    registers[0xF] = 0;
    uint8_t height = opcode & 0x000F;
    for (int row = 0; row < height; row++)
    {
        uint8_t nthSpriteByte = memory[I + row];
        for (int p = 0; p < SPR_WIDTH; p++)
        {
            uint8_t pixel = nthSpriteByte & (0x80u >> p);

            // if pixel is 0, there would be no change
            // we do not want to wrap if we go over the screen
            uint8_t i = (x + p) % DIS_WIDTH;
            uint8_t j = (y + row) % DIS_HEIGHT;
            if (pixel)
            {
                uint32_t offset = j * DIS_WIDTH + i;
                uint32_t videoPixel = pixels.at(offset);
                registers[0xf] |= (videoPixel == UINT32_MAX) && pixel;
                pixels[offset] ^= UINT32_MAX;
            }
        }
    }
    refreshFlag = true;
}

bool Chip8::getRefreshFlag()
{
    return refreshFlag;
}

void Chip8::getPixels(uint32_t * dst)
{
    for(uint32_t i = 0; i < pixels.size(); i++)
        dst[i] = pixels[i];
}

void Chip8::setKey(int index, uint8_t state)
{
    if (index > 0)
        keys[index] = state;
}
