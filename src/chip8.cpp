#include <cstdlib>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "chip8.hpp"
using namespace std;

#define VX(opcode) V[(opcode & VX_MASK) >> 8]
#define VY(opcode) V[(opcode & VY_MASK) >> 4]
#define VF V[0xF]

unsigned char chip8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::initialize()
{
    pc = 0x200; // Program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;     // Reset stack pointer

    // Clear display
    // Clear stack
    for (size_t i = 0; i < 16; i++)
        stack[i] = 0;
    
    // Clear registers V0-VF
    for (size_t i = 0; i < 16; i++)
        V[i] = 0;
    
    // Clear memory
    for (size_t i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;
    
    // Load fontset
    for (int i = 0; i < 80; ++i)
        memory[0x50 + i] = chip8_fontset[i];

   for (size_t i = 0; i < DIS_WIDTH * DIS_HEIGHT; i++)
        pixels[i] = 0;

    for (size_t i = 0; i < 16; i++)
        key[i] = 0;
}

void Chip8::emulateCycle()
{
    refreshFlag = false;
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    // Decode & Execute Opcode
    switch ((opcode & 0xF000) >> 12)
    {
    case 0x0:
        switch (opcode)
        {
        case 0x00E0: // clear display
            memset(pixels, 0, sizeof(pixels));
            break;
        case 0x00EE: // return
            pc = stack[--sp];
            break;
        default: // 0x0NNN call machine routine at address NNN
            break;
        }
        break;
    case 0x1: // jump to address at NNN
        pc = opcode & NNN_MASK;
        break;
    case 0x2: // call subroutine at NNN
        stack[sp++] = pc + 2;
        pc = opcode & NNN_MASK;
        break;
    case 0x3: // Skip next instr if VX == NN
        if (VX(opcode) == (opcode & NN_MASK))
            pc += 4;
        else
            pc += 2;
        break;
    case 0x4: // Skip next instr if VX != NN
        if (VX(opcode) != (opcode & NN_MASK))
            pc += 4;
        else
            pc += 2;
        break;
    case 0x5: // Skip next instr if VX == VY
        if (VX(opcode) == VY(opcode))
            pc += 4;
        else
            pc += 2;
        break;
    case 0x6:
        VX(opcode) = opcode & NN_MASK;
        pc += 2;
        break;
    case 0x7:
        VX(opcode) += opcode & NN_MASK;
        pc += 2;
        break;
    case 0x8:
        execALU(opcode);
        pc += 2;
        break;
    case 0x9:
        if (VX(opcode) != VY(opcode))
            pc += 4;
        else
            pc += 2;
        break;
    case 0xA:
        I = opcode & NNN_MASK;
        pc += 2;
        break;
    case 0xB:
        pc = V[0] + opcode & NNN_MASK;
        break;
    case 0xC:
        VX(opcode) = rand() & (opcode & NN_MASK);
        pc += 2;
        break;
    case 0xD:
        drawSprite(opcode);
        pc += 2;
        break;
    case 0xE:
        switch (opcode & NN_MASK)
        {
        case 0x9E:
            if (key[VX(opcode)])
                pc += 4;
            else
                pc += 2; 
            break;
        case 0xA1:
            if (!key[VX(opcode)])
                pc += 4;
            else
                pc += 2;
            break;
        }
        break;

    case 0xF:
        switch (opcode & NN_MASK)
        {
        case 0x07:
            VX(opcode) = delay_timer;
            break;
        case 0x0A:
            // find pressed key and store it in Vx
            for (size_t i = 0; i < 16; i++)
            {
                if (key[i])
                {
                    pc += 2;
                    VX(opcode) = i;
                    break;
                }
            }
            // clear key presses
            for (size_t i = 0; i < 16; i++)
                key[i] = 0;
            // add negative offset, when += 2 returns to this instruction, ie block
            pc -= 2;
            break;
        case 0x15:
            delay_timer = VX(opcode);
            break;
        case 0x18:
            sound_timer = VX(opcode);
            break;
        case 0x1E:
            I += VX(opcode);
            break;
        case 0x29:
            I = 0x50 + (VX(opcode) * 5);
            break;
        case 0x33:
            memory[I] = VX(opcode) / 100;
            memory[I + 1] = (VX(opcode) / 10) % 10;
            memory[I + 2] = VX(opcode) % 10;
            break;
        case 0x55:
            for (size_t i = 0; i <= (opcode & VX_MASK) >> 8; i++)
                memory[I + i] = V[i];
            break;
        case 0x65:
            for (size_t i = 0; i <= (opcode & VX_MASK) >> 8; i++)
                V[i] = memory[I + i];
            break;
        }
        pc += 2;
        break;
    default:
        printf("invalid opcode %04x", opcode);
        break;
    }

end_cycle:
    // Update timers
    delay_timer = max(delay_timer - 1, 0);
    sound_timer = max(sound_timer - 1, 0);
}

void Chip8::execALU(unsigned short opcode)
{
    switch (opcode & 0x000F)
    {
    case 0x0: // Vx = Vy
        VX(opcode) = VY(opcode);
        break;
    case 0x1: // Vx |= Vy
        VX(opcode) |= VY(opcode);
        break;
    case 0x2: // Vx &= Vy
        VX(opcode) &= VY(opcode);
        break;
    case 0x3: // Vx xor Vy
        VX(opcode) ^= VY(opcode);
        break;
    case 0x4:
        VF = VX(opcode) + VY(opcode) > 255;
        VX(opcode) = VX(opcode) + VY(opcode);
        break;
    case 0x5:
        VF = VX(opcode) > VY(opcode);
        VX(opcode) = VX(opcode) - VY(opcode);
        break;
    case 0x6: // Vx >>= 1
        VF = VX(opcode) & 0x1;
        VX(opcode) >>= 1;
        break;
    case 0x7:
        VF = VY(opcode) > VX(opcode);
        VX(opcode) = VY(opcode) + VX(opcode) & 0x0F;
        break;
    case 0xE:
        VF = VX(opcode) & 0x80;
        VX(opcode) <<= 1;
        break;
    }
}

void Chip8::loadGame(char *game)
{
    ifstream myfile;
    myfile.open(game, ios::binary);
    if (myfile.is_open())
        myfile.read((char *)&memory[0x200], 3584);
    else
        printf("error loading game: %s", game);
    myfile.close();
}

void Chip8::drawSprite(unsigned short opcode)
{
    char x = VX(opcode) % DIS_WIDTH;
    char y = VY(opcode) % DIS_HEIGHT;
    VF = 0;
    char rows = opcode & 0x000F;
    for (size_t n = 0; n < rows; n++)
    {
        char nthSpriteByte = memory[I + n];
        for (size_t p = 0; p < SPR_WIDTH; p++)
        {
            char pixel = (nthSpriteByte >> (8 - p - 1)) & 1;
            if (x + p <= DIS_WIDTH && y + n <= DIS_HEIGHT) // do not wrap
            {
                int offset = (y + n) * DIS_WIDTH + x + p;
                VF |= pixels[offset] && pixel;
                pixels[offset] ^= pixel;
            }
        }
    }
    refreshFlag = true;
}

char Chip8::getRefreshFlag()
{
    return refreshFlag;
}

void Chip8::getGfx(Uint32 *result)
{
    int width = 64, height = 32;
    for (size_t i = 0; i < width * height; i++)
        result[i] = pixels[i] ? UINT32_MAX : 0;
}

void Chip8::setKey(int index, unsigned char state)
{
    if (index > 0)
        key[index] = state;
}
