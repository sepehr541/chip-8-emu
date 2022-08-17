#include "chip8.hpp"

void Chip8::OpFX07()
{
    setRegisterAtX(delay_timer);
    pc += 2;
}

void Chip8::OpFX0A()
{
    bool pressed = false;
    // find pressed key and store it in Vx
    for (size_t i = 0; i < 16; i++)
    {
        if (keys[i])
        {
            pressed = true;
            setRegisterAtX(i);
            break;
        }
    }

    // clear key presses
    std::fill(keys.begin(), keys.end(), 0);

    // if nothing pressed, do not inc pc to block
    pc += pressed ? 2 : 0;
}
void Chip8::OpFX15()
{
    delay_timer = getRegisterAtX();
    pc += 2;
}

void Chip8::OpFX18()
{
    sound_timer = getRegisterAtX();
    pc += 2;
}

void Chip8::OpFX1E()
{
    I += getRegisterAtX();
    pc += 2;
}

void Chip8::OpFX29()
{
    I = 0x50 + (getRegisterAtX() * 5);
    pc += 2;
}

void Chip8::OpFX33()
{
    memory[I] = getRegisterAtX() / 100;
    memory[I + 1] = (getRegisterAtX() / 10) % 10;
    memory[I + 2] = getRegisterAtX() % 10;

    pc += 2;
}

void Chip8::OpFX55()
{
    for (size_t i = 0; i <= getX(); i++)
        memory[I + i] = registers.at(i);

    pc += 2;
}
void Chip8::OpFX65()
{
    for (size_t i = 0; i <= getX(); i++)
        registers[i] = memory[I + i];

    pc += 2;
}
