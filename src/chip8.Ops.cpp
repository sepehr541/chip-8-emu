#include "chip8.hpp"
#include <functional>

void Chip8::Op0()
{
    std::invoke(OpTable0.at(opcode), *this);
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
    pc += getRegisterAtX() == (opcode & NN_MASK) ? 4 : 2;
}

void Chip8::Op4XNN()
{
    pc += getRegisterAtX() != (opcode & NN_MASK) ? 4 : 2;
}

void Chip8::Op5XYN()
{
    pc += getRegisterAtX() == getRegisterAtY() ? 4 : 2;
}

void Chip8::Op6XNN()
{
    setRegisterAtX(opcode & NN_MASK);
    pc += 2;
}

void Chip8::Op7XNN()
{
    setRegisterAtX(getRegisterAtX() + (opcode & NN_MASK));
    pc += 2;
}

void Chip8::Op8XY()
{
    uint16_t offset = opcode & 0x000Fu;
    std::invoke(OpTable8XY[offset], *this);
    pc += 2;
}

void Chip8::Op9XY0()
{
    pc += getRegisterAtX() != getRegisterAtY() ? 4 : 2;
}

void Chip8::OpANNN()
{
    I = opcode & NNN_MASK;
    pc += 2;
}

void Chip8::OpBNNN()
{
    pc = registers.at(0) + opcode & NNN_MASK;
}

void Chip8::OpCXNN()
{
    setRegisterAtX(rand() & (opcode & NN_MASK));
    pc += 2;
}

void Chip8::OpDXYN()
{
    drawSprite(opcode);
    pc += 2;
}

void Chip8::OpEX()
{
    uint16_t offset = opcode & NN_MASK;
    std::invoke(OpTableEX.at(offset), *this);
}

void Chip8::OpFX()
{
    uint16_t offset = opcode & NN_MASK;
    std::invoke(OpTableFX.at(offset), *this);
}