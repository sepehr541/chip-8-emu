#ifndef CHIP8_H
#define CHIP8_H

#define MEM_SIZE 4096
#define STACK_SIZE 16
#define NNN_MASK 0x0FFF
#define VX_MASK 0x0F00
#define VY_MASK 0x00F0
#define NN_MASK 0x00FF
#define OP_SIZE 2
#define DIS_WIDTH 64
#define DIS_HEIGHT 34
#define SPR_WIDTH 8

class Chip8
{
    unsigned char memory[MEM_SIZE];               // 4K memory
    unsigned char V[16];                          // V0 - VF registers
    unsigned short I;                             // 12-bit index register
    unsigned short pc;                            // program counter
    unsigned char pixels[DIS_WIDTH * DIS_HEIGHT]; // display pixels' state (1 or 0)
    bool refreshFlag;

    // timers
    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[STACK_SIZE];
    unsigned short sp;

    unsigned char key[16];

    unsigned short opcode;

public:
    void initialize();
    void emulateCycle();
    void loadGame(char *game);
    char getRefreshFlag();
    void getGfx(Uint32*);
    void handleKeyUp();
    void handleKeyDown();
    void setKey(int index, unsigned char state);

private:
    void execALU(unsigned short opcode);
    void drawSprite(unsigned short opcode);
};
#endif