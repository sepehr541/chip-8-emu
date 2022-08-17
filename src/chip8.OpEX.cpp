#include "chip8.hpp"

void Chip8::OpEX9E()
{
    pc += keys.at(getRegisterAtX()) ? 4 : 2;
}

void Chip8::OpEXA1()
{
    pc += !keys.at(getRegisterAtX()) ? 4 : 2;
}
