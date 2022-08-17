#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include "clock.hpp"

class Chip8
{
public:
    ///
    /// Mark - Constant
    ///

    // Configurations
    static constexpr uint32_t MEM_SIZE = 4096;
    static constexpr uint32_t STACK_SIZE = 16;
    static constexpr uint32_t REGISTER_COUNT = 16;
    static constexpr uint32_t KEYS_COUNT = 16;

    // Masks
    static constexpr uint32_t NNN_MASK = 0x0FFF;
    static constexpr uint32_t VX_MASK = 0x0F00;
    static constexpr uint32_t VY_MASK = 0x00F0;
    static constexpr uint32_t VX_SHIFT = 8u;
    static constexpr uint32_t VY_SHIFT = 4u;
    static constexpr uint32_t NN_MASK = 0x00FF;
    static constexpr uint32_t OP_SIZE = 2;

    // Display settings
    static constexpr uint32_t DIS_WIDTH = 64;
    static constexpr uint32_t DIS_HEIGHT = 34;
    static constexpr uint32_t DIS_PIXELS = DIS_WIDTH * DIS_HEIGHT;
    static constexpr uint32_t SPR_WIDTH = 8;

    // fontset
    static constexpr uint8_t chip8_fontset[80] =
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

    ///
    /// Mark - Members
    ///

private:
    // 4K memory
    std::vector<uint8_t> memory;

    // V0 - VF registers
    std::vector<uint8_t> registers;

    // 12-bit index register
    uint16_t I;

    // program counter
    uint16_t pc;

    // display pixels' state, UINT32_MAX (on) or 0 (off)
    std::vector<uint32_t> pixels;

    // flag for redraw
    bool refreshFlag;

    // timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // 60Hz clock for timers
    Clock clock;

    // stack
    std::vector<uint16_t> stack;

    // stack pointer
    uint16_t sp;

    // 0 - F key map
    std::vector<uint8_t> keys;

    // current opcode
    uint16_t opcode;

    ///
    /// Mark - Constuctors
    ///
public:
    Chip8() : memory(MEM_SIZE, 0), registers(REGISTER_COUNT, 0), pixels(DIS_PIXELS, 0), stack(STACK_SIZE, 0), keys(KEYS_COUNT, 0), clock(60) {};

    ///
    /// Mark - Functions
    ///

public:
    ///
    /// initialize the
    ///
    void initialize();

    ///
    /// run a cycle
    ///
    void emulateCycle();

    ///
    /// load game into memory from ROM file
    ///
    void loadGame(std::string game);

    ///
    /// Get current refreshFlag
    ///
    /// @return refreshFlag
    bool getRefreshFlag();

    ///
    /// get graphics pixels
    ///
    std::vector<uint32_t>& getPixels();

    ///
    /// set key state in key map
    ///
    /// @param index index of the key (0 - F)
    /// @param state state of the key (up or down)
    ///
    void setKey(int index, uint8_t state);

    ///
    /// decrement the timers at 60Hz
    ///
    void decTimers();

    ///
    /// Mark - Helpers
    ///

private:
    // type alias for pointer-to-member-function for Chip-8 class
    typedef void (Chip8::*Chip8Func)();

    // Operations
    void Op0();
    void Op1NNN();
    void Op2NNN();
    void Op3XNN();
    void Op4XNN();
    void Op5XYN();
    void Op6XNN();
    void Op7XNN();
    void Op8XY();
    void Op9XY0();
    void OpANNN();
    void OpBNNN();
    void OpCXNN();
    void OpDXYN();
    void OpEX();
    void OpFX();

    // Op table
    Chip8Func OpTable[17] =
        {
            &Chip8::Op0,
            &Chip8::Op1NNN,
            &Chip8::Op2NNN,
            &Chip8::Op3XNN,
            &Chip8::Op4XNN,
            &Chip8::Op5XYN,
            &Chip8::Op6XNN,
            &Chip8::Op7XNN,
            &Chip8::Op8XY,
            &Chip8::Op9XY0,
            &Chip8::OpANNN,
            &Chip8::OpBNNN,
            &Chip8::OpCXNN,
            &Chip8::OpDXYN,
            &Chip8::OpEX,
            &Chip8::OpFX,
    };

    ///
    // ALU 8XY*
    ///
    void Op8XY0();
    void Op8XY1();
    void Op8XY2();
    void Op8XY3();
    void Op8XY4();
    void Op8XY5();
    void Op8XY6();
    void Op8XY7();
    void Op8XYE();

    std::unordered_map<uint16_t, Chip8Func> OpTable8XY{
        {0x0, &Chip8::Op8XY0},
        {0x1, &Chip8::Op8XY1},
        {0x2, &Chip8::Op8XY2},
        {0x3, &Chip8::Op8XY3},
        {0x4, &Chip8::Op8XY4},
        {0x5, &Chip8::Op8XY5},
        {0x6, &Chip8::Op8XY6},
        {0x7, &Chip8::Op8XY7},
        {0xE, &Chip8::Op8XYE},
    };

    ///
    // Op 0***
    ///
    
    // ignoring call to machine function
    // void Op0NNN();
    
    void Op00E0();
    void Op00EE();

    std::unordered_map<uint16_t, Chip8Func> OpTable0{
        // { 0x0, &Chip8::Op0NNN },
        {0x00E0, &Chip8::Op00E0},
        {0x00EE, &Chip8::Op00EE},
    };

    ///
    // Op EX**
    ///
    void OpEX9E();
    void OpEXA1();

    std::unordered_map<uint16_t, Chip8Func> OpTableEX{
        {0x9E, &Chip8::OpEX9E},
        {0xA1, &Chip8::OpEXA1},
    };

    ///
    // Op FX**
    ///
    void OpFX07();
    void OpFX0A();
    void OpFX15();
    void OpFX18();
    void OpFX1E();
    void OpFX29();
    void OpFX33();
    void OpFX55();
    void OpFX65();

    std::unordered_map<uint16_t, Chip8Func> OpTableFX = {
        {0x07, &Chip8::OpFX07},
        {0x0A, &Chip8::OpFX0A},
        {0x15, &Chip8::OpFX15},
        {0x18, &Chip8::OpFX18},
        {0x1E, &Chip8::OpFX1E},
        {0x29, &Chip8::OpFX29},
        {0x33, &Chip8::OpFX33},
        {0x55, &Chip8::OpFX55},
        {0x65, &Chip8::OpFX65},
    };

    /// 
    /// bound check the index given for register
    ///
    /// @param index index of the register to access
    /// @throws error if out of bound
    ///
    inline void boundCheckRegisters(uint32_t index)
    {
        if (index < 0 || index > REGISTER_COUNT - 1)
        {
            throw "Bad Register Index";
        }
    }

    ///
    /// get the value of X from opcode NXYN
    /// 
    /// @returns the value of X from opcode
    ///
    inline uint32_t getX()
    {
        return (opcode & VX_MASK) >> VX_SHIFT;
    }

    ///
    /// get the value of Y from opcode NXYN
    /// 
    /// @returns the value of Y from opcode
    ///
    inline uint32_t getY()
    {
        return (opcode & VY_MASK) >> VY_SHIFT;
    }

    ///
    /// set the register indexed by X from opcode NXYN to value
    /// 
    /// @param value value to put in register
    ///
    inline void setRegisterAtX(uint8_t value)
    {
        uint32_t index = getX();
        boundCheckRegisters(index);
        registers[index] = value;
    };


    ///
    /// set the register indexed by Y from opcode NXYN to value
    /// 
    /// @param value value to put in register
    ///
    inline void setRegisterAtY(uint8_t value)
    {
        uint32_t index = getY();
        boundCheckRegisters(index);
        registers[index] = value;
    };

    ///
    /// get the register indexed by X from opcode NXYN to value
    /// 
    /// @returns value in register at index X
    ///
    inline uint8_t getRegisterAtX()
    {
        return registers.at(getX());
    };

    ///
    /// get the register indexed by Y from opcode NXYN to value
    /// 
    /// @returns value in register at index Y
    ///
    inline uint8_t getRegisterAtY()
    {
        return registers.at(getY());
    };

    ///
    /// set register at indexed 0xF (the flag register)
    /// 
    /// @param value value to set the register to
    ///
    inline void setFlagRegister(uint8_t value)
    {
        registers[0xF] = value;
    }
};
#endif