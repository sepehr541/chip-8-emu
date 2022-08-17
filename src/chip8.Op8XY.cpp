#include "chip8.hpp"

void Chip8::Op8XY0()
{
    setRegisterAtX(getRegisterAtY());
}

void Chip8::Op8XY1()
{
    uint8_t v = getRegisterAtX() | getRegisterAtY();
    setRegisterAtX(v);
}

void Chip8::Op8XY2()
{
    uint8_t v = getRegisterAtX() & getRegisterAtY();
    setRegisterAtX(v);
}

void Chip8::Op8XY3()
{
    uint8_t v = getRegisterAtX() ^ getRegisterAtY();
    setRegisterAtX(v);
}

void Chip8::Op8XY4()
{
    setFlagRegister(getRegisterAtX() + getRegisterAtY() > 255);
    
    uint8_t v = getRegisterAtX() + getRegisterAtY();
    setRegisterAtX(v);
}

void Chip8::Op8XY5()
{
    setFlagRegister(getRegisterAtX() > getRegisterAtY());
    
    uint8_t v = getRegisterAtX() - getRegisterAtY();
    setRegisterAtX(v);
}
void Chip8::Op8XY6()
{
    setFlagRegister(getRegisterAtX() & 0x1);
    
    uint8_t v = getRegisterAtX() >> 1;
    setRegisterAtX(v);
}
void Chip8::Op8XY7()
{
    setFlagRegister(getRegisterAtY() > getRegisterAtX());
    
    uint8_t v = getRegisterAtY() + getRegisterAtX() & 0x0F;
    setRegisterAtX(v);
}
void Chip8::Op8XYE()
{
    setFlagRegister(getRegisterAtX() & 0x80);
    
    uint8_t v = getRegisterAtX() << 1;
    setRegisterAtX(v);
}