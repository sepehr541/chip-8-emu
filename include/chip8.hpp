#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>

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
    static constexpr uint32_t NN_MASK = 0x00FF;
    static constexpr uint32_t OP_SIZE = 2;

    // Display settings
    static constexpr uint32_t DIS_WIDTH = 64;
    static constexpr uint32_t DIS_HEIGHT = 34;
    static constexpr uint32_t SPR_WIDTH = 8;

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
    uint8_t memory[MEM_SIZE];

    // V0 - VF registers
    uint8_t V[REGISTER_COUNT];

    // 12-bit index register
    uint16_t I;

    // program counter
    uint16_t pc;

    // display pixels' state, UINT32_MAX (on) or 0 (off)
    uint32_t pixels[DIS_WIDTH * DIS_HEIGHT];

    // flag for redraw
    bool refreshFlag;

    // timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // stack
    uint16_t stack[STACK_SIZE];

    // stack pointer
    uint16_t sp;

    // 0 - F key map
    uint8_t keys[KEYS_COUNT];

    // current opcode
    uint16_t opcode;

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
    /// copy video grpahics pixels to given array
    ///
    uint32_t *getPixels();

    ///
    /// handle key release
    ///
    void handleKeyUp();

    ///
    /// handle key press
    ///
    void handleKeyDown();

    ///
    /// set key state in key map
    ///
    /// @param index index of the key (0 - F)
    /// @param state state of the key (up or down)
    ///
    void setKey(int index, uint8_t state);

    ///
    /// Mark - Helpers
    ///

private:
    
	typedef void (Chip8::*Chip8Func)();

    // Operations
    void Op0NNN();
    void Op1NNN();
    void Op2NNN();
    void Op3XNN();
    void Op4XNN();
    void Op5XYN();
    void Op6XNN();
    void Op7XNN();
    void Op8XYN();
    void Op9XY0();
    void OpANNN();
    void OpBNNN();
    void OpCXNN();
    void OpDXYN();
    void OpEXNN();
    void OpFXNN();

    // Op table
    Chip8Func OpTable[17];
    Chip8Func OpTable0[3];
    Chip8Func OpTable8[16];
    Chip8Func OpTableF[9];

    void setupOpTable();

    void execALU(uint16_t opcode);
    void drawSprite(uint16_t opcode);
};
#endif