#include "chip8.hpp"

void Chip8::Op00E0()
{
    std::fill(pixels.begin(), pixels.end(), 0);
}

void Chip8::Op00EE()
{
    pc = stack.at(--sp);
}
