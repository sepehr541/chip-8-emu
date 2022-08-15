#include <cstdlib>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "chip8.hpp"
using namespace std;

#define VX(opcode) V[(opcode & VX_MASK) >> 8]
#define VY(opcode) V[(opcode & VY_MASK) >> 4]
#define VF V[0xF]

inline void setArrayToZero(void *array, uint32_t size)
{
    memset(array, 0, size);
}

void Chip8::setupOpTable()
{
    // Operations
    OpTable[0x0] = &Chip8::Op0NNN;
    OpTable[0x1] = &Chip8::Op1NNN;
    OpTable[0x2] = &Chip8::Op2NNN;
    OpTable[0x3] = &Chip8::Op3XNN;
    OpTable[0x4] = &Chip8::Op4XNN;
    OpTable[0x5] = &Chip8::Op5XYN;
    OpTable[0x6] = &Chip8::Op6XNN;
    OpTable[0x7] = &Chip8::Op7XNN;
    OpTable[0x8] = &Chip8::Op8XYN;
    OpTable[0x9] = &Chip8::Op9XY0;
    OpTable[0xA] = &Chip8::OpANNN;
    OpTable[0xB] = &Chip8::OpBNNN;
    OpTable[0xC] = &Chip8::OpCXNN;
    OpTable[0xD] = &Chip8::OpDXYN;
    OpTable[0xE] = &Chip8::OpEXNN;
    OpTable[0xF] = &Chip8::OpFXNN;
}

// Operations
void Chip8::Op0NNN()
{
    switch (opcode)
    {
    case 0x00E0: // clear display
        setArrayToZero(pixels, sizeof(pixels));
        break;
    case 0x00EE: // return
        pc = stack[--sp];
        break;
    default: // 0x0NNN call machine routine at address NNN
        stack[sp++] = pc + 2;
        pc = opcode & NNN_MASK;
        break;
    }
}

void Chip8::Op1NNN()
{
    pc = opcode & NNN_MASK;
}

void Chip8::Op2NNN()
{
    stack[sp++] = pc + 2;
    pc = opcode & NNN_MASK;
}

void Chip8::Op3XNN()
{
    pc += VX(opcode) == (opcode & NN_MASK) ? 4 : 2;
}

void Chip8::Op4XNN()
{
    pc += VX(opcode) != (opcode & NN_MASK) ? 4 : 2;
}

void Chip8::Op5XYN()
{
    pc += VX(opcode) == VY(opcode) ? 4 : 2;
}

void Chip8::Op6XNN()
{
    VX(opcode) = opcode & NN_MASK;
    pc += 2;
}

void Chip8::Op7XNN()
{
    VX(opcode) += opcode & NN_MASK;
    pc += 2;
}

void Chip8::Op8XYN()
{
    execALU(opcode);
    pc += 2;
}

void Chip8::Op9XY0()
{
    pc += VX(opcode) != VY(opcode) ? 4 : 2;
}

void Chip8::OpANNN()
{
    I = opcode & NNN_MASK;
    pc += 2;
}

void Chip8::OpBNNN()
{
    pc = V[0] + opcode & NNN_MASK;
}

void Chip8::OpCXNN()
{
    VX(opcode) = rand() & (opcode & NN_MASK);
    pc += 2;
}

void Chip8::OpDXYN()
{
    drawSprite(opcode);
    pc += 2;
}

void Chip8::OpEXNN()
{
    switch (opcode & NN_MASK)
    {
    case 0x9E:
        pc += keys[VX(opcode)] ? 4 : 2;
        break;
    case 0xA1:
        pc += !keys[VX(opcode)] ? 4 : 2;
        break;
    }
}

void Chip8::OpFXNN()
{
    switch (opcode & NN_MASK)
        {
        case 0x07:
            VX(opcode) = delay_timer;
            break;
        case 0x0A:
            // find pressed key and store it in Vx
            for (size_t i = 0; i < 16; i++)
            {
                if (keys[i])
                {
                    pc += 2;
                    VX(opcode) = i;
                    break;
                }
            }
            // clear key presses
            for (size_t i = 0; i < 16; i++)
                keys[i] = 0;
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
}

void Chip8::initialize()
{
    pc = 0x200; // Program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;     // Reset stack pointer

    setupOpTable();

    // Clear stack
    setArrayToZero(stack, STACK_SIZE);

    // Clear registers V0-VF
    setArrayToZero(V, REGISTER_COUNT);

    // Clear memory
    setArrayToZero(memory, MEM_SIZE);

    // Load fontset
    for (int i = 0; i < 80; ++i)
        memory[0x50 + i] = chip8_fontset[i];

    // Clear display
    setArrayToZero(pixels, sizeof(pixels));

    // Clear keys
    setArrayToZero(keys, KEYS_COUNT);
}

void Chip8::emulateCycle()
{
    refreshFlag = false;
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    
    // Decode & Execute Opcode
    ((*this).*(OpTable[(opcode & 0xF000u) >> 12u]))();

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

void Chip8::loadGame(std::string game)
{
    ifstream myfile;
    myfile.open(game, ios::binary);
    if (myfile.is_open())
        myfile.read((char *)&memory[0x200], 3584);
    else
        std::cout << "error loading game: " << game << std::endl;
    myfile.close();
}

void Chip8::drawSprite(unsigned short opcode)
{
    char x = VX(opcode) % DIS_WIDTH;
    char y = VY(opcode) % DIS_HEIGHT;
    VF = 0;
    char height = opcode & 0x000F;
    for (size_t row = 0; row < height; row++)
    {
        uint8_t nthSpriteByte = memory[I + row];
        for (size_t p = 0; p < SPR_WIDTH; p++)
        {
            uint8_t pixel = nthSpriteByte & (0x80u >> p);

            // if pixel is 0, there would be no change
            // we do not want to wrap if we go over the screen
            if (pixel && x + p <= DIS_WIDTH && y + row <= DIS_HEIGHT)
            {
                int offset = (y + row) * DIS_WIDTH + x + p;
                uint32_t videoPixel = pixels[offset];
                VF |= (videoPixel == UINT32_MAX) && pixel;
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

uint32_t *Chip8::getPixels()
{
    return this->pixels;
}

void Chip8::setKey(int index, unsigned char state)
{
    if (index > 0)
        keys[index] = state;
}
