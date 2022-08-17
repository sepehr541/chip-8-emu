#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <SDL.h>
#include <unordered_map>

class Chip8;

class KeyboardDriver
{
    ///
    /// Mark - Members
    ///
private:
    Chip8 &chip8;

    std::unordered_map<SDL_Keycode, int> keyMap = {
        {SDLK_1, 0x1},
        {SDLK_2, 0x2},
        {SDLK_3, 0x3},
        {SDLK_4, 0xC},
        {SDLK_q, 0x4},
        {SDLK_w, 0x5},
        {SDLK_e, 0x6},
        {SDLK_r, 0xD},
        {SDLK_a, 0x7},
        {SDLK_s, 0x8},
        {SDLK_d, 0x9},
        {SDLK_f, 0xE},
        {SDLK_z, 0xA},
        {SDLK_x, 0x0},
        {SDLK_c, 0xB},
        {SDLK_v, 0xF},
    };

    ///
    /// Mark - Constructors
    ///

public:
    KeyboardDriver(Chip8 &chip8): chip8(chip8) {};

    ///
    /// Mark - Functions
    ///

public:
    bool handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) > 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return true;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                chip8.setKey(keyMap.at(event.key.keysym.sym), event.type == SDL_KEYDOWN);
                break;
            }
        }
        return false;
    }
};

#endif